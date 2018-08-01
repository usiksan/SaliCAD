/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Common short time

  All time in seconds 32-bit from begin of year 2018.
*/
#ifndef SDTIME2X_H
#define SDTIME2X_H

#include <QDateTime>

//Start time is begin of year 2018 in seconds from epoch
#define SD_TIME_2X_START 1514764800l

class SdTime2x
  {
  public:
    //Current moment [Текущий глобальный момент]
    static qint32    current();

    //Conversion to-from epich [Взаимодействие со временем относительно эпохи]
    //Convert to epoch [Преобразовать в эпоху]
    static qint64    toEpoch( qint32 tm );

    //Convert from epoch [Преобразовать из эпохи]
    static qint32    fromEpoch( qint64 tm );


    //Get real utc date [Получить реальную дату]
    static QDateTime toUtc( qint32 tm );

    //Get real local date [Получить реальную локальную дату]
    static QDateTime toLocal( qint32 tm );

    //Convert from QDataTime [Преобразовать из момента]
    static qint32    fromDateTime( const QDateTime tm );


    //Convert to string representation of time
    static QString   toLocalString( qint32 tm );
  };

#endif // SDTIME2X_H
