package main

import (
	"crypto/rand"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"io"
	"net"
	"os"
	"path/filepath"
	"sort"
	"strconv"
	"strings"
	"sync"
	"time"
)

// Конфигурация сервера
type Config struct {
	Port               int           `json:"port"`
	DataDir            string        `json:"data_dir"`
	CloudIDPrefix      string        `json:"cloud_id_prefix"`
	InactiveTimeout    time.Duration `json:"inactive_timeout"`
	CleanupInterval    time.Duration `json:"cleanup_interval"`
	MaxCloudIDLength   int           `json:"max_cloud_id_length"`
	MaxEntityIDLength  int           `json:"max_entity_id_length"`
	MaxFileSize        int64         `json:"max_file_size"`
}

// Структура для хранения информации о CloudID
type CloudInfo struct {
	CloudID        string    `json:"cloud_id"`
	LastAccess     time.Time `json:"last_access"`
	LastModification time.Time `json:"last_modification"`
}

// Структуры запросов и ответов
type Request struct {
	Type     string `json:"type"`
	CloudID  string `json:"cloud_id,omitempty"`
	FromTime int64  `json:"from_time,omitempty"` // Время последней синхронизации в миллисекундах
	EntityID string `json:"entity_id,omitempty"` // Идентификатор сущности
	Data     []byte `json:"data,omitempty"`      // Данные сущности для загрузки
	Version  string `json:"version,omitempty"`   // Версия сущности
}

type Response struct {
	Success    bool        `json:"success"`
	Error      string      `json:"error,omitempty"`
	CloudID    string      `json:"cloud_id,omitempty"`
	Entities   []EntityInfo `json:"entities,omitempty"`
	Data       []byte      `json:"data,omitempty"`
	LastModification int64 `json:"last_modification,omitempty"`
}

type EntityInfo struct {
	ID      string `json:"id"`
	Version string `json:"version"`
}

// Сервер
type SyncServer struct {
	config     Config
	cloudMutex sync.Map // Для блокировки доступа к разным облакам
	stopChan   chan struct{}
	wg         sync.WaitGroup
}

// Генерация нового CloudID
func generateCloudID(prefix string) string {
	bytes := make([]byte, 7)
	rand.Read(bytes)
	return fmt.Sprintf("%s-%s", prefix, hex.EncodeToString(bytes))
}

// Проверка корректности CloudID
func isValidCloudID(cloudID, prefix string) bool {
	if !strings.HasPrefix(cloudID, prefix+"-") {
		return false
	}

	idPart := strings.TrimPrefix(cloudID, prefix+"-")
	if len(idPart) != 14 { // 7 байт в hex = 14 символов
		return false
	}

	_, err := hex.DecodeString(idPart)
	return err == nil
}

// Получение пути к каталогу облака
func (s *SyncServer) getCloudPath(cloudID string) string {
	return filepath.Join(s.config.DataDir, cloudID)
}

// Получение пути к файлу сущности
func (s *SyncServer) getEntityPath(cloudID, entityID, version string) string {
	filename := fmt.Sprintf("%s-%s.salix", entityID, version)
	return filepath.Join(s.config.DataDir, cloudID, filename)
}

// Обновление времени доступа к облаку
func (s *SyncServer) updateCloudAccessTime(cloudID string) {
	cloudPath := s.getCloudPath(cloudID)
	accessFile := filepath.Join(cloudPath, ".last_access")

	// Создаем файл с текущим временем
	os.WriteFile(accessFile, []byte(time.Now().Format(time.RFC3339)), 0644)
}

// Обновление времени последнего изменения
func (s *SyncServer) updateCloudModificationTime(cloudID string) {
	cloudPath := s.getCloudPath(cloudID)
	modFile := filepath.Join(cloudPath, ".last_modification")

	// Создаем файл с текущим временем
	os.WriteFile(modFile, []byte(time.Now().Format(time.RFC3339)), 0644)
}

// Получение времени последнего изменения облака
func (s *SyncServer) getCloudModificationTime(cloudID string) time.Time {
	cloudPath := s.getCloudPath(cloudID)
	modFile := filepath.Join(cloudPath, ".last_modification")

	data, err := os.ReadFile(modFile)
	if err != nil {
		return time.Time{}
	}

	t, err := time.Parse(time.RFC3339, string(data))
	if err != nil {
		return time.Time{}
	}

	return t
}

// Создание каталога облака при необходимости
func (s *SyncServer) ensureCloudDirectory(cloudID string) error {
	cloudPath := s.getCloudPath(cloudID)

	// Проверяем существование каталога
	if _, err := os.Stat(cloudPath); os.IsNotExist(err) {
		// Создаем каталог
		err := os.MkdirAll(cloudPath, 0755)
		if err != nil {
			return fmt.Errorf("failed to create cloud directory: %v", err)
		}

		// Создаем файлы времени доступа и модификации
		s.updateCloudAccessTime(cloudID)
		s.updateCloudModificationTime(cloudID)
	}

	return nil
}

// Получение списка сущностей из каталога
func (s *SyncServer) getCloudEntities(cloudID string, fromTime int64) ([]EntityInfo, error) {
	cloudPath := s.getCloudPath(cloudID)

	// Проверяем существование каталога
	if _, err := os.Stat(cloudPath); os.IsNotExist(err) {
		return []EntityInfo{}, nil
	}

	// Читаем все файлы в каталоге
	files, err := os.ReadDir(cloudPath)
	if err != nil {
		return nil, fmt.Errorf("failed to read cloud directory: %v", err)
	}

	// Карта для группировки сущностей по ID (оставляем только последние версии)
	entityMap := make(map[string]EntityInfo)

	for _, file := range files {
		filename := file.Name()

		// Пропускаем служебные файлы
		if strings.HasPrefix(filename, ".") {
			continue
		}

		// Проверяем расширение файла
		if !strings.HasSuffix(filename, ".salix") {
			continue
		}

		// Извлекаем ID и версию из имени файла
		baseName := strings.TrimSuffix(filename, ".salix")
		parts := strings.Split(baseName, "-")
		if len(parts) < 2 {
			continue
		}

		// Последний элемент - версия
		version := parts[len(parts)-1]
		// Проверяем формат версии (8 hex символов)
		if len(version) != 8 {
			continue
		}

		// Проверяем, что версия является hex числом
		if _, err := strconv.ParseUint(version, 16, 32); err != nil {
			continue
		}

		// Формируем ID сущности (все части кроме последней)
		entityID := strings.Join(parts[:len(parts)-1], "-")

		// Получаем информацию о файле
		info, err := file.Info()
		if err != nil {
			continue
		}

		// Проверяем время модификации файла
		if fromTime > 0 && info.ModTime().UnixMilli() <= fromTime {
			continue
		}

		// Проверяем, есть ли уже более новая версия этой сущности
		if existing, exists := entityMap[entityID]; exists {
			// Сравниваем версии как hex числа
			existingVer, _ := strconv.ParseUint(existing.Version, 16, 32)
			newVer, _ := strconv.ParseUint(version, 16, 32)

			if newVer > existingVer {
				entityMap[entityID] = EntityInfo{
					ID:      entityID,
					Version: version,
				}
			}
		} else {
			entityMap[entityID] = EntityInfo{
				ID:      entityID,
				Version: version,
			}
		}
	}

	// Преобразуем карту в срез
	result := make([]EntityInfo, 0, len(entityMap))
	for _, entity := range entityMap {
		result = append(result, entity)
	}

	// Сортируем по ID для детерминированного результата
	sort.Slice(result, func(i, j int) bool {
		return result[i].ID < result[j].ID
	})

	return result, nil
}

// Обработчик запроса на получение нового CloudID
func (s *SyncServer) handleNewCloudID() Response {
	cloudID := generateCloudID(s.config.CloudIDPrefix)

	return Response{
		Success: true,
		CloudID: cloudID,
	}
}

// Обработчик запроса на получение списка сущностей
func (s *SyncServer) handleGetEntities(cloudID string, fromTime int64) Response {
	// Проверяем CloudID
	if !isValidCloudID(cloudID, s.config.CloudIDPrefix) {
		return Response{
			Success: false,
			Error:   "Invalid CloudID format",
		}
	}

	// Получаем мьютекс для этого облака
	mutex, _ := s.cloudMutex.LoadOrStore(cloudID, &sync.Mutex{})
	mutex.(*sync.Mutex).Lock()
	defer mutex.(*sync.Mutex).Unlock()

	// Обновляем время доступа
	s.updateCloudAccessTime(cloudID)

	// Получаем список сущностей
	entities, err := s.getCloudEntities(cloudID, fromTime)
	if err != nil {
		return Response{
			Success: false,
			Error:   fmt.Sprintf("Failed to get entities: %v", err),
		}
	}

	// Получаем время последнего изменения
	lastMod := s.getCloudModificationTime(cloudID)

	return Response{
		Success:          true,
		Entities:         entities,
		LastModification: lastMod.UnixMilli(),
	}
}

// Обработчик запроса на получение сущности
func (s *SyncServer) handleGetEntity(cloudID, entityID, version string) Response {
	// Проверяем CloudID
	if !isValidCloudID(cloudID, s.config.CloudIDPrefix) {
		return Response{
			Success: false,
			Error:   "Invalid CloudID format",
		}
	}

	// Получаем мьютекс для этого облака
	mutex, _ := s.cloudMutex.LoadOrStore(cloudID, &sync.Mutex{})
	mutex.(*sync.Mutex).Lock()
	defer mutex.(*sync.Mutex).Unlock()

	// Обновляем время доступа
	s.updateCloudAccessTime(cloudID)

	// Проверяем существование каталога облака
	cloudPath := s.getCloudPath(cloudID)
	if _, err := os.Stat(cloudPath); os.IsNotExist(err) {
		return Response{
			Success: true,
			Data:    []byte{},
		}
	}

	// Формируем путь к файлу
	entityPath := s.getEntityPath(cloudID, entityID, version)

	// Читаем файл
	data, err := os.ReadFile(entityPath)
	if err != nil {
		if os.IsNotExist(err) {
			return Response{
				Success: true,
				Data:    []byte{},
			}
		}
		return Response{
			Success: false,
			Error:   fmt.Sprintf("Failed to read entity file: %v", err),
		}
	}

	return Response{
		Success: true,
		Data:    data,
	}
}

// Обработчик запроса на запись сущности
func (s *SyncServer) handlePutEntity(cloudID, entityID, version string, data []byte) Response {
	// Проверяем CloudID
	if !isValidCloudID(cloudID, s.config.CloudIDPrefix) {
		return Response{
			Success: false,
			Error:   "Invalid CloudID format",
		}
	}

	// Проверяем размер файла
	if int64(len(data)) > s.config.MaxFileSize {
		return Response{
			Success: false,
			Error:   "File size exceeds limit",
		}
	}

	// Проверяем формат версии
	if len(version) != 8 {
		return Response{
			Success: false,
			Error:   "Version must be 8 hex characters",
		}
	}

	// Проверяем, что версия является hex числом
	if _, err := strconv.ParseUint(version, 16, 32); err != nil {
		return Response{
			Success: false,
			Error:   "Version must be a hex number",
		}
	}

	// Получаем мьютекс для этого облака
	mutex, _ := s.cloudMutex.LoadOrStore(cloudID, &sync.Mutex{})
	mutex.(*sync.Mutex).Lock()
	defer mutex.(*sync.Mutex).Unlock()

	// Обновляем время доступа
	s.updateCloudAccessTime(cloudID)

	// Создаем каталог облака при необходимости
	if err := s.ensureCloudDirectory(cloudID); err != nil {
		return Response{
			Success: false,
			Error:   fmt.Sprintf("Failed to create cloud directory: %v", err),
		}
	}

	// Проверяем существование более новой версии этой сущности
	entities, err := s.getCloudEntities(cloudID, 0)
	if err != nil {
		return Response{
			Success: false,
			Error:   fmt.Sprintf("Failed to check existing entities: %v", err),
		}
	}

	// Ищем существующую версию этой сущности
	for _, entity := range entities {
		if entity.ID == entityID {
			// Сравниваем версии
			existingVer, _ := strconv.ParseUint(entity.Version, 16, 32)
			newVer, _ := strconv.ParseUint(version, 16, 32)

			if newVer < existingVer {
				// Версия старше существующей - игнорируем
				return Response{
					Success: true,
				}
			} else if newVer == existingVer {
				// Та же версия - перезаписываем
				break
			} else {
				// Новая версия - удаляем старую
				oldPath := s.getEntityPath(cloudID, entityID, entity.Version)
				os.Remove(oldPath)
				break
			}
		}
	}

	// Сохраняем файл
	entityPath := s.getEntityPath(cloudID, entityID, version)
	if err := os.WriteFile(entityPath, data, 0644); err != nil {
		return Response{
			Success: false,
			Error:   fmt.Sprintf("Failed to write entity file: %v", err),
		}
	}

	// Обновляем время последнего изменения
	s.updateCloudModificationTime(cloudID)

	return Response{
		Success: true,
	}
}

// Обработка входящего соединения
func (s *SyncServer) handleConnection(conn net.Conn) {
	defer conn.Close()

	// Читаем запрос
	var req Request
	decoder := json.NewDecoder(conn)
	if err := decoder.Decode(&req); err != nil {
		if err != io.EOF {
			resp := Response{
				Success: false,
				Error:   fmt.Sprintf("Failed to parse request: %v", err),
			}
			json.NewEncoder(conn).Encode(resp)
		}
		return
	}

	// Обрабатываем запрос в зависимости от типа
	var resp Response

	switch req.Type {
	case "new_cloud_id":
		resp = s.handleNewCloudID()

	case "get_entities":
		if req.CloudID == "" {
			resp = Response{Success: false, Error: "CloudID is required"}
		} else {
			resp = s.handleGetEntities(req.CloudID, req.FromTime)
		}

	case "get_entity":
		if req.CloudID == "" || req.EntityID == "" {
			resp = Response{Success: false, Error: "CloudID and EntityID are required"}
		} else {
			resp = s.handleGetEntity(req.CloudID, req.EntityID, req.Version)
		}

	case "put_entity":
		if req.CloudID == "" || req.EntityID == "" || req.Version == "" || req.Data == nil {
			resp = Response{Success: false, Error: "CloudID, EntityID, Version and Data are required"}
		} else {
			resp = s.handlePutEntity(req.CloudID, req.EntityID, req.Version, req.Data)
		}

	default:
		resp = Response{
			Success: false,
			Error:   fmt.Sprintf("Unknown request type: %s", req.Type),
		}
	}

	// Отправляем ответ
	encoder := json.NewEncoder(conn)
	if err := encoder.Encode(resp); err != nil {
		fmt.Printf("Failed to send response: %v\n", err)
	}
}

// Очистка неактивных облаков
func (s *SyncServer) cleanupInactiveClouds() {
	ticker := time.NewTicker(s.config.CleanupInterval)
	defer ticker.Stop()

	for {
		select {
		case <-ticker.C:
			s.doCleanup()
		case <-s.stopChan:
			return
		}
	}
}

// Выполнение очистки неактивных облаков
func (s *SyncServer) doCleanup() {
	dataDir := s.config.DataDir

	// Читаем все каталоги в data_dir
	entries, err := os.ReadDir(dataDir)
	if err != nil {
		fmt.Printf("Failed to read data directory for cleanup: %v\n", err)
		return
	}

	for _, entry := range entries {
		if !entry.IsDir() {
			continue
		}

		cloudID := entry.Name()

		// Проверяем формат CloudID
		if !isValidCloudID(cloudID, s.config.CloudIDPrefix) {
			continue
		}

		// Проверяем время последнего доступа
		cloudPath := s.getCloudPath(cloudID)
		accessFile := filepath.Join(cloudPath, ".last_access")

		data, err := os.ReadFile(accessFile)
		if err != nil {
			// Если файла нет, удаляем каталог
			os.RemoveAll(cloudPath)
			s.cloudMutex.Delete(cloudID)
			fmt.Printf("Removed inactive cloud: %s (no access file)\n", cloudID)
			continue
		}

		lastAccess, err := time.Parse(time.RFC3339, string(data))
		if err != nil {
			// Если не удалось прочитать время, удаляем
			os.RemoveAll(cloudPath)
			s.cloudMutex.Delete(cloudID)
			fmt.Printf("Removed inactive cloud: %s (invalid access time)\n", cloudID)
			continue
		}

		// Проверяем, не истекло ли время неактивности
		if time.Since(lastAccess) > s.config.InactiveTimeout {
			os.RemoveAll(cloudPath)
			s.cloudMutex.Delete(cloudID)
			fmt.Printf("Removed inactive cloud: %s (last access: %v)\n", cloudID, lastAccess)
		}
	}
}

// Запуск сервера
func (s *SyncServer) Start() error {
	// Создаем каталог данных, если не существует
	if err := os.MkdirAll(s.config.DataDir, 0755); err != nil {
		return fmt.Errorf("failed to create data directory: %v", err)
	}

	// Запускаем очистку неактивных облаков
	s.wg.Add(1)
	go func() {
		defer s.wg.Done()
		s.cleanupInactiveClouds()
	}()

	// Запускаем TCP сервер
	addr := fmt.Sprintf(":%d", s.config.Port)
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		return fmt.Errorf("failed to start server: %v", err)
	}
	defer listener.Close()

	fmt.Printf("Sync server started on port %d\n", s.config.Port)
	fmt.Printf("Data directory: %s\n", s.config.DataDir)
	fmt.Printf("CloudID prefix: %s\n", s.config.CloudIDPrefix)

	// Обрабатываем входящие соединения
	for {
		conn, err := listener.Accept()
		if err != nil {
			select {
			case <-s.stopChan:
				return nil
			default:
				fmt.Printf("Failed to accept connection: %v\n", err)
				continue
			}
		}

		go s.handleConnection(conn)
	}
}

// Остановка сервера
func (s *SyncServer) Stop() {
	close(s.stopChan)
	s.wg.Wait()
}

// Создание нового сервера
func NewSyncServer(config Config) *SyncServer {
	return &SyncServer{
		config:   config,
		stopChan: make(chan struct{}),
	}
}

func main() {
	// Конфигурация по умолчанию
	config := Config{
		Port:               8080,
		DataDir:            "./cloud_data",
		CloudIDPrefix:      "Cloud",
		InactiveTimeout:    30 * 24 * time.Hour, // 30 дней
		CleanupInterval:    1 * time.Hour,       // Каждый час
		MaxCloudIDLength:   50,
		MaxEntityIDLength:  255,
		MaxFileSize:        10 * 1024 * 1024, // 10 MB
	}

	// Пытаемся прочитать конфигурацию из файла
	if configData, err := os.ReadFile("config.json"); err == nil {
		if err := json.Unmarshal(configData, &config); err != nil {
			fmt.Printf("Failed to parse config file: %v\n", err)
		}
	} else {
		// Создаем файл конфигурации по умолчанию
		configJSON, _ := json.MarshalIndent(config, "", "  ")
		os.WriteFile("config.json", configJSON, 0644)
		fmt.Println("Created default config file: config.json")
	}

	// Создаем и запускаем сервер
	server := NewSyncServer(config)

	// Обрабатываем сигналы для graceful shutdown
	go func() {
		// В реальном приложении здесь была бы обработка сигналов
		// Для примера просто ждем
	}()

	// Запускаем сервер
	if err := server.Start(); err != nil {
		fmt.Printf("Server error: %v\n", err)
		os.Exit(1)
	}
}

