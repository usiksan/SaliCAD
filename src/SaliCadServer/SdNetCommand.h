/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Net command list and net structures

  On server stored object name table and objects.
  Object name table contains object identification information. Each record of table is constant lenght.
  So, to select appropriate record need to move file table pointer.
*/
#ifndef SDNETCOMMAND_H
#define SDNETCOMMAND_H

//Net command list
#define SDC_GET_INFO        1 //Get server information
#define SDC_INFO            2 //Server information
                              //128byte - textual information
#define SDC_GET_UPGRADE     1 //Get current version for product class
                              // 16bit product class
#define SDC_UPGRADE_INFO    2 //Current upgrade version for product class
                              // 16bit product class
                              // 16bit major version
                              // 16bit minor version
#define SDC_GET_OBJECT_LIST 3 //Get object list newer than desired date
                              // 32bit start time
#define SDC_OBJECT_LIST     4 //Object list fragment
                              // SDC_OBJECT_COUNT_IN_LIST * SDR_LENGHT bytes - table fragment
#define SDC_GET_OBJECT      5 //Get object with desired name
                              // 128bit - registration key
                              // SDR_LENGHT object record
#define SDC_OBJECT          6 //Object
                              // 32bit - object lenght
                              // 128bit - registration key
                              // SDR_LENGHT object record
                              // object data
#define SDC_ERROR_COMMAND   7 //Invalid command code
#define SDC_ERROR_KEY       8 //Invalid registration key

#define SDC_OBJECT_COUNT_IN_LIST 32
#define SDC_INFO_LENGHT          128

//Object table
#define SDR_LENGHT       1024 //Full lenght of one record of object name table
#define SDR_F_TIME_REC      0 //32bit time of record write
#define SDR_F_TIME          4 //32bit time of object revision
#define SDR_F_AUTHOR        8 //32bit author id
#define SDR_F_RESERV       12 //Reserved area
#define SDR_F_NAME        256 //Object name in UTF-8

#define MAX_PACKET        (SDR_LENGHT * SDC_OBJECT_COUNT_IN_LIST + 5)


#define timeOffsetConstant 1000000000L

#endif // SDNETCOMMAND_H
