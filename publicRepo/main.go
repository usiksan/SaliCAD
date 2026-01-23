package main

import (
  "encoding/json"
  "fmt"
  "log"
  "net"
  "os"
  "path/filepath"
  "sync"
  "time"

  "gopkg.in/timshannon/bolthold.v1"
  )



// Конфигурация
type Config struct {
  Port        int    `json:"port"`
  DataDir     string `json:"data_dir"`
  DBPath      string `json:"db_path"`
  }

// Структуры данных
type ElementRef struct {
  ElementID   string `boltholdKey:"ElementID"` // идентификатор сущности
  AuthorID    string
  DislikeCount int32
  CreateTime   int32
  StoreTime    int32
  }

//Список авторов
type Author struct {
  AuthorID     string `boltholdKey:"AuthorID"` // идентификатор автора
  PrivateKey   string
  Name         string
  Email        string
  ElementCount  int32
  }

type Issue struct {
  ElementID string `boltholdKey:"ElementID"` // идентификатор сущности
  Message   string
  Status     int32
  }


// Сервер
type SalixServer struct {
    config     Config
    store      *bolthold.Store
    listener   net.Listener
    wg         sync.WaitGroup
    shutdown   chan struct{}
}




// SV_TIME_2X_START = 1514764800 = 1 января 2018, 00:00:00 UTC
const SV_TIME_2X_START int64 = 1514764800


// FromEpoch преобразует из epoch time (аналог SvTime2x::fromEpoch())
func timeFromEpoch(tm int64) int32 {
  // При отсутствии времени возвращаем ноль обозначающий тоже отсутствие времени
  if tm == 0 {
    return 0
    }
  return int32(tm - SV_TIME_2X_START)
  }



// Current возвращает текущий глобальный момент (аналог SvTime2x::current())
func timeCurrent() int32 {
  return timeFromEpoch(time.Now().Unix())
  }






// Создание нового сервера
func NewSalixServer(cfg Config) (*SalixServer, error) {
  // Создаем каталог для данных если его нет
  if err := os.MkdirAll(cfg.DataDir, 0755); err != nil {
    return nil, fmt.Errorf("failed to create data directory: %v", err)
    }

  // Открываем/создаем базу данных
  store, err := bolthold.Open(cfg.DBPath, 0666, nil)
  if err != nil {
    return nil, fmt.Errorf("failed to open database: %v", err)
    }
/*
  // Инициализируем базу (создаем индексы и т.д.)
  if err := initializeDatabase(store); err != nil {
    store.Close()
    return nil, fmt.Errorf("failed to initialize database: %v", err)
    }
  */
  return &SalixServer{
    config:   cfg,
    store:    store,
    shutdown: make(chan struct{}),
    }, nil
  }




/*
// Инициализация базы данных
func initializeDatabase(store *bolthold.Store) error {
  // BoltHold автоматически создает индексы для полей с тегом boltholdIndex
  // при первом сохранении записи. Но мы можем выполнить дополнительные
  // инициализационные операции здесь.

  // Например, создаем системные записи если их нет
  return store.Update(func(tx *bolthold.Tx) error {
    // Проверяем наличие системного автора (администратора)
    var admin Author
    err := tx.Get("system", &admin)
    if err != nil {
      // Создаем системного автора
      admin = Author {
        AuthorID:    "system",
        Name:        "salix",
        Email:       "",
        ElementCount: 0,
        }
      if err := tx.Insert(admin.ID, &admin); err != nil {
        return fmt.Errorf("failed to create system author: %v", err)
        }
      log.Println("Created system author")
      }
    // Можно добавить другие инициализационные операции
    return nil
    })
  }
 */



// Запуск сервера
func (s *SalixServer) Start() error {
  // Запускаем TCP сервер
  addr := fmt.Sprintf(":%d", s.config.Port)
  listener, err := net.Listen("tcp", addr)
  if err != nil {
    return fmt.Errorf("failed to start server: %v", err)
    }
  s.listener = listener

  log.Printf("Salix public server started on port %d", s.config.Port)
  log.Printf("Database: %s", s.config.DBPath)

  // Основной цикл принятия соединений
  for {
    select {
      case <-s.shutdown:
        return nil
      default:
        conn, err := listener.Accept()
        if err != nil {
          select {
            case <-s.shutdown:
              return nil
            default:
              log.Printf("Error accepting connection: %v", err)
              continue
            }
          }

        // Обрабатываем соединение в отдельной горутине
        s.wg.Add(1)
        go s.handleConnection(conn)
      }
    }
  }




// Обработчик соединения
func (s *SalixServer) handleConnection(conn net.Conn) {
  defer s.wg.Done()
  defer conn.Close()

  remoteAddr := conn.RemoteAddr().String()
  log.Printf("New connection from %s", remoteAddr)

  // Создаем обработчик для этого соединения
  handler := NewConnectionHandler(s.store, conn)

  // Обрабатываем запросы пока соединение активно
  handler.ProcessRequests()

  log.Printf("Connection closed: %s", remoteAddr)
  }




// Остановка сервера
func (s *SalixServer) Stop() {
  close(s.shutdown)

  // Останавливаем listener
  if s.listener != nil {
    s.listener.Close()
    }

  // Ждем завершения всех обработчиков
  s.wg.Wait()

  // Закрываем базу данных
  if s.store != nil {
    s.store.Close()
    }

  log.Println("Server stopped")
  }




// Обработчик соединения
type ConnectionHandler struct {
  store *bolthold.Store
  conn  net.Conn
  }



func NewConnectionHandler(store *bolthold.Store, conn net.Conn) *ConnectionHandler {
    return &ConnectionHandler{
        store: store,
        conn:  conn,
    }
}



// Обработка запросов
func (h *ConnectionHandler) ProcessRequests() {
  // Буфер для чтения
  buf := make([]byte, 4096)

    for {
        // Читаем данные
        n, err := h.conn.Read(buf)
        if err != nil {
            // Клиент закрыл соединение или ошибка
            break
        }

        // Обрабатываем запрос
        response := h.handleRequest(buf[:n])

        // Отправляем ответ
        if _, err := h.conn.Write(response); err != nil {
            log.Printf("Failed to send response: %v", err)
            break
        }
    }
}



// Заглушка для обработки запросов
func (h *ConnectionHandler) handleRequest(data []byte) []byte {
    // TODO: Реальная обработка запросов
    // Пока просто возвращаем подтверждение

    // Пример: парсим JSON запрос
    var req map[string]interface{}
    if err := json.Unmarshal(data, &req); err != nil {
        // Не JSON - возвращаем ошибку
        errorResp := map[string]interface{}{
            "success": false,
            "error":   "Invalid JSON request",
        }
        resp, _ := json.Marshal(errorResp)
        return resp
    }

    // Определяем тип запроса
    action, _ := req["action"].(string)

    // Заглушка для разных типов запросов
    switch action {
    case "ping":
        return h.handlePing()
    case "get_element":
        return h.handleGetElement(req)
    case "get_author":
        return h.handleGetAuthor(req)
    case "report_issue":
        return h.handleReportIssue(req)
    default:
        errorResp := map[string]interface{}{
            "success": false,
            "error":   fmt.Sprintf("Unknown action: %s", action),
        }
        resp, _ := json.Marshal(errorResp)
        return resp
    }
}




// Примеры обработчиков (заглушки)
func (h *ConnectionHandler) handlePing() []byte {
    resp := map[string]interface{}{
        "success": true,
        "message": "pong",
        "time":    time.Now().Unix(),
    }
    data, _ := json.Marshal(resp)
    return data
}




func (h *ConnectionHandler) handleGetElement(req map[string]interface{}) []byte {
    elementID, _ := req["element_id"].(string)

    var element ElementRef
    err := h.store.Get(elementID, &element)

    resp := make(map[string]interface{})
    if err != nil {
        resp["success"] = false
        resp["error"] = "Element not found"
    } else {
        resp["success"] = true
        resp["element"] = element
    }

    data, _ := json.Marshal(resp)
    return data
}




func (h *ConnectionHandler) handleGetAuthor(req map[string]interface{}) []byte {
    authorID, _ := req["author_id"].(string)

    var author Author
    err := h.store.Get(authorID, &author)

    resp := make(map[string]interface{})
    if err != nil {
        resp["success"] = false
        resp["error"] = "Author not found"
    } else {
        // Не возвращаем приватный ключ
        safeAuthor := map[string]interface{}{
            "id":    author.AuthorID,
            "name":  author.Name,
            "email": author.Email,
            "count": author.ElementCount,
        }
        resp["success"] = true
        resp["author"] = safeAuthor
    }

    data, _ := json.Marshal(resp)
    return data
}

func (h *ConnectionHandler) handleReportIssue(req map[string]interface{}) []byte {
    elementID, _ := req["element_id"].(string)
    message, _ := req["message"].(string)

    issue := Issue{
        ElementID: elementID,
        Message:   message,
        Status:    0, // новый
    }

    err := h.store.Insert(issue.ElementID, &issue)

    resp := make(map[string]interface{})
    if err != nil {
        resp["success"] = false
        resp["error"] = fmt.Sprintf("Failed to report issue: %v", err)
    } else {
        resp["success"] = true
        resp["issue_id"] = issue.ElementID
    }

    data, _ := json.Marshal(resp)
    return data
}

// Вспомогательные функции
func fileExists(path string) bool {
    _, err := os.Stat(path)
    return err == nil
}



func loadConfig() Config {
  // Конфигурация по умолчанию
  cfg := Config{
    Port:    8081,
    DataDir: "./library",
    }
  cfg.DBPath = filepath.Join(cfg.DataDir, "salix.db")

  // Можно загрузить из файла конфигурации
  configFile := "salix-config.json"
  if fileExists(configFile) {
    data, err := os.ReadFile(configFile)
    if err == nil {
      json.Unmarshal(data, &cfg)
      }
    }

  return cfg
  }



func main() {
  test()

  // Загружаем конфигурацию
  config := loadConfig()

  // Создаем и запускаем сервер
  server, err := NewSalixServer(config)
  if err != nil {
    log.Fatalf("Failed to create server: %v", err)
    }

  // Обработка сигналов для graceful shutdown
  // (в реальном приложении нужно добавить обработку SIGINT, SIGTERM)

  // Запускаем сервер
  if err := server.Start(); err != nil {
    log.Printf("Server error: %v", err)
    }

  server.Stop()
  }
