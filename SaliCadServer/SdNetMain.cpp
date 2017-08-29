/*
  Project "Electronic schematic and pcb CAD"
  Проект "Канал удаленного управления и мониторинга"
 Автор
  Сибилев А.С.
  www.saliLab.com
  www.saliLab.ru
 Описание
   Основное назначение - передаточное звено-канал между slave и master.

   Подключение к каналу осуществляется по инициативе удаленного компьютера.
   После подключения компьютер шлет свой статус (slave или master) и эти
   данные размещаются в таблицах.

   master может прислать требование коммуникации с заданным slave. В этом
   случае между ними организуется взаимная передача данных.

 История
  29.08.2017 создан на базе SvStudio
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SdNetCommand.h"

#define TITLE                "Sibilev SaliCAD Server"
#define VERSION              4

#define MAX_CONNECTION      64

#define MAX_PACKET        1096

FILE *objectTableFile;
FILE *keyListFile;

struct Connection {
    int         mSocket; //Сокет для подключения
    bool        mLinked; //Состояние подключения

    void init( int sock ) { mSocket = sock; mLinked = false; }

    void close();

    int  recv( void *dest ) { return ::recv( mSocket, dest, MAX_PACKET, 0 ); }

    int  send( const void *src, int size ) { return ::send( mSocket, src, size, 0 ); }
  };


void Connection::close()
  {
  ::close(mSocket);
  mSocket = 0;
  }



Connection connections[MAX_CONNECTION];


int getCurTime() {
  return (int)(time(nullptr) -  timeOffsetConstant);
  }


int getInt16( const uint8_t *src )
  {
  return (src[0] << 8) | (src[1]);
  }

void setInt16( int val, uint8_t *dst )
  {
  dst[0] = (val >> 8) & 0xff;
  dst[1] = val & 0xff;
  }





int getInt32( const uint8_t *src )
  {
  int val;
  val = src[3];
  val <<= 8;
  val |= src[2];
  val <<= 8;
  val |= src[1];
  val <<= 8;
  val |= src[0];
  return val;
  }

void setInt32( uint8_t *dest, int val )
  {
  dest[0] = val & 0xff;
  dest[1] = (val >> 8) & 0xff;
  dest[2] = (val >> 16) & 0xff;
  dest[3] = (val >> 24) & 0xff;
  }




bool ckeckKey( const char *key ) {
  //Перебрать все записи в файле ключей и сравнить с заданным ключом
  char buf[512];
  int res = 0;
  for( long pos = 0; res; pos += 512 ) {
    //Прочитать очередную запись
    flockfile( keyListFile );
    fseek(keyListFile, pos, SEEK_SET);
    res = fread( buf, 1, 512, keyListFile );
    funlockfile( keyListFile );
    if( res ) return false;
    if( strcmp( buf, key ) == 0 ) return true;
    }
  return false;
  }



void addKey( const char *key ) {
  //Проверить наличие ключа в списке, если есть, то ничего не добавляем
  if( ckeckKey(key) )
    return;
  //Подготовить буфер для ключа
  char buf[512];
  memset( buf, 0, 512 );
  //Скопировать ключ в буфер
  strncpy( buf, key, 511 );

  //Дозаписать ключ
  flockfile( keyListFile );
  fseek(keyListFile, 0, SEEK_END);
  fwrite( buf, 1, 512, keyListFile );
  funlockfile( keyListFile );
  }




bool readObjectIdBin( char *dest, int pos ) {
  //Выполнить чтение одной записи
  int res;
  flockfile( objectTableFile );
  res = fseek( objectTableFile, pos * SDR_LENGHT, SEEK_SET );
  if( res == 0 )
    res = fread( dest, SDR_LENGHT, 1, objectTableFile ) != 1;
  funlockfile( objectTableFile );
  return res == 0;
  }



int findObjectIdBin( const char id[] ) {
  //Поиск заданной записи в файле
  char buf[SDR_LENGHT];
  for( int pos = 0; readObjectIdBin( buf, pos ); pos++ ) {
    //Выполнить проверку идентичности
    int i = 0;

    //Начальные поля
    while( i < SDR_F_RESERV )
      if( id[i] != buf[i] ) break;
      else i++;

    //Если проверку начальных полей прошли, то проверяем наименование
    if( i == SDR_F_RESERV ) {
      i = SDR_F_NAME;
      while( id[i] && buf[i] && i < SDR_LENGHT )
        if( id[i] != buf[i] ) break;
        else i++;

      //Если дошли до конца наименования, то детектируем совпадение
      if( id[i] == 0 && buf[i] == 0 )
        //При совпадении возврат с номером записи
        return pos;
      }
    }
  return -1;
  }



int findObjectIdNameAuthor( const char *name, int author ) {
  //Поиск заданной записи в файле
  char buf[SDR_LENGHT];
  for( int pos = 0; readObjectIdBin( buf, pos ); pos++ ) {
    //Выполнить проверку идентичности
    if( getInt32( (const uint8_t *)(buf+SDR_F_AUTHOR) ) == author ) {
      //Автор совпадает, проверить имя

      int i = SDR_F_NAME;
      while( buf[i] && name[i - SDR_F_NAME] && i < SDR_LENGHT )
        if( buf[i] != name[i - SDR_F_NAME] ) break;
        else i++;

      //Если дошли до конца наименования, то детектируем совпадение
      if( buf[i] == 0 && name[i - SDR_F_NAME] == 0 )
        //При совпадении возврат с номером записи
        return pos;
      }
    }
  return -1;
  }




void addObjectIdBin( const char id[], int pos ) {
  flockfile( objectTableFile );
  if( pos < 0 ) {
    if( findObjectIdBin(id) < 0 ) {
      //Запись не найдена, добавить
      fseek( objectTableFile, 0, SEEK_END );
      fwrite( id, SDR_LENGHT, 1, objectTableFile );
      }
    }
  else {
    //Записать по точному адресу
    long off = pos;
    fseek( objectTableFile, off * SDR_LENGHT, SEEK_SET );
    fwrite( id, SDR_LENGHT, 1, objectTableFile );
    }
  funlockfile( objectTableFile );
  }




bool addObjectIdNameAuthorTime( const char *name, int author, int time ) {
  char buf[SDR_LENGHT];
  flockfile( objectTableFile );
  int pos = findObjectIdNameAuthor( name, author );
  if( pos >= 0 ) {
    //Запись найдена, проверить время создания
    readObjectIdBin( buf, pos );
    if( getInt32( (const uint8_t*)(buf + SDR_F_TIME) ) < time ) {
      //Изменить время, время записи и обновить
      setInt32( (uint8_t*)(buf+SDR_F_TIME), time );
      setInt32( (uint8_t*)(buf+SDR_F_TIME_REC), getCurTime() );
      addObjectIdBin( buf, pos );
      }
    }
  else {
    //Запись не найдена, создать и добавить
    memset( buf, 0, SDR_LENGHT );
    setInt32( (uint8_t*)(buf+SDR_F_TIME), time );
    setInt32( (uint8_t*)(buf+SDR_F_TIME_REC), getCurTime() );
    setInt32( (uint8_t*)(buf+SDR_F_AUTHOR), author );
    strncpy( buf + SDR_F_NAME, name, SDR_LENGHT - SDR_F_AUTHOR - 1 );
    addObjectIdBin( buf, -1 );
    }
  funlockfile( objectTableFile );
  }



static void *connection(void *vptr_args) {
  Connection  *cn = ((Connection*)vptr_args);
  char buf[1096];

  printf("local loop started\n");

  //main connection loop
  while(1) {
    int bytes_read = cn->recv( &buf );
    if(bytes_read <= 0) break;



    //Get server information
    if( buf[0] == SDC_GET_INFO ) {
      //Server information
      //128byte - textual information
      buf[0] = SDC_INFO;
      sprintf( buf+1, "%s %d", TITLE, VERSION );
      cn->send( buf, SDC_INFO_LENGHT  );
      }



    //Get current version for product class
    else if( buf[0] == SDC_GET_UPGRADE ) {
      // 16bit product class
      int cls = getInt16( (const uint8_t*) (buf+1) );

      //Current upgrade version for product class
      // 16bit product class
      // 16bit major version
      // 16bit minor version
      buf[0] = SDC_UPGRADE_INFO;
      setInt16( cls, (uint8_t*) (buf+1) );
      setInt16( 0, (uint8_t*) (buf+3) );
      setInt16( 0, (uint8_t*) (buf+5) );
      cn->send( buf, 7 );
      }





    //Get object list newer than desired date
    else if( buf[0] == SDC_GET_OBJECT_LIST ) {
      // 32bit start time
      int time = getInt32( (const uint8_t*) (buf+1) );

      //Открыть файл фаблицы только для чтения
      //Найти место с первой записью большей чем time
      //Передать наименьшее: 16 записей или до конца файла
      //Object list fragment
      buf[0] = SDC_OBJECT_LIST;
      // SDC_OBJECT_COUNT_IN_LIST * SDR_LENGHT bytes - table fragment
      int len = 0;

      cn->send( buf, 1 + len );
      }






    //Get object with desired name
    else if( buf[0] == SDC_GET_OBJECT ) {
      // 128bit - registration key
      // SDR_LENGHT object record

      //Пройти по списку доступных ключей, если все ок, то передать объект
      bool ok = false;
      //Иначе передать ошибку
      if( ok ) {
        //Передать объект, для этого мы ищем объект
        //Object
        buf[0] = SDC_OBJECT;
        // 128bit - registration key
        // SDR_LENGHT object record
        // 32bit - object lenght
  //      setInt32( 0, buf + SDR_LENGHT + 128 + 1 );
        // object data
        }
      else {
        //Передать ошибку
        buf[0] = SDC_ERROR_KEY; //Invalid registration key
        cn->send( buf, 1 );
        }
      }




    else if( buf[0] == SDC_OBJECT ) {

      }




    else {
      //Invalid command code
      buf[0] = SDC_ERROR_COMMAND;
      cn->send( buf, 1 );
      cn->close();
      break;
      }



    }


  printf("Connection %d closed\n", (int)(cn - connections) );
  cn->close();
  return 0;
  }


int main() {
  int listener;
  struct sockaddr_in addr;

  //Начальное сообщение
  printf( TITLE );
  printf( "\nVersion %d\n\n", VERSION );

  //Открыть файл ключей доступа
  keyListFile = fopen( "keyList.dat", "r+" );
  if( keyListFile == nullptr )
    keyListFile = fopen( "keyList.dat", "w+" );

  if( keyListFile == nullptr ) {
    printf("Error. Can't create or open keyList.dat");
    return 0;
    }

  addKey( "SaliLAB" );

  //Открыть файл объектов
  objectTableFile = fopen( "objectTable.dat", "r+" );
  if( objectTableFile == nullptr )
    objectTableFile = fopen( "objectTable.dat", "w+" );


  //define listener
  listener = socket(AF_INET, SOCK_STREAM, 0);
  if(listener < 0) {
    perror("socket");
    exit(1);
    }

  //bind listener
  addr.sin_family = AF_INET;
  addr.sin_port = htons(1970);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(2);
    }

  //register listener
  listen(listener, 1);

  //connection loop
  printf( "connection loop started\n");
  while(1) {
    int sock = accept(listener, NULL, NULL);
    if(sock < 0) {
      printf("accept connection failed\n");
      continue;
      }

    //find cell for connection
    int i;
    for( i = 0; i < MAX_CONNECTION; i++ )
      if( connections[i].mSocket == 0 )
        //cell is free
        break;

    if( i >= MAX_CONNECTION ) {
      //no free cell
      printf("no free cell\n");
      close(sock);
      continue;
      }

    //fill cell
    connections[i].init( sock );

    //create thread with connection
    pthread_t th;

    if( pthread_create(&th, NULL, connection, connections + i ) != 0 ) {
      printf("fail to create thread\n");
      connections[i].close();
      continue;
      }
    }

  return 0;
  }
