/*
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
  24.02.2016 создан
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "SdNetCommand.h"

#define TITLE                "Sibilev SaliCAD"
#define VERSION              4

#define MAX_CONNECTION      64

#define MAX_PACKET     1000000

struct Connection {
    int         mSocket;
    int         mId;
    int         mPassw;

    void init( int sock ) { mSocket = sock; mId = mPassw = 0; }

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
  int keyFile = open( "keys.dat", 0 );
  if( keyFile ) {

    }
  return false;
  }




static void *connection(void *vptr_args) {
  Connection  *cn = ((Connection*)vptr_args);
  char buf[1000000];

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
      int cls = getInt16( buf+1 );

      //Current upgrade version for product class
      // 16bit product class
      // 16bit major version
      // 16bit minor version
      buf[0] = SDC_UPGRADE_INFO;
      setInt16( cls, buf+1 );
      setInt16( 0, buf+3 );
      setInt16( 0, buf+5 );
      cn->send( buf, 7 );
      }





    //Get object list newer than desired date
    else if( buf[0] == SDC_GET_OBJECT_LIST ) {
      // 32bit start time
      int time = getInt32(buf+1);

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
        setInt32( 0, buf + SDR_LENGHT + 128 + 1 );
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


  printf("Connection %d closed\n", cn - connections );
  cn->close();
  return 0;
  }


int main() {
  int listener;
  struct sockaddr_in addr;

  //Начальное сообщение
  printf( TITLE );
  printf( "\nVersion %d\n\n", VERSION );

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
