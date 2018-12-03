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
#include "SdTime2x.h"
#include <QDebug>



//Преобразовать в эпоху
qint32 SdTime2x::current()
  {
  return fromEpoch( QDateTime::currentSecsSinceEpoch() );
  }



//Convert to epoch [Преобразовать в эпоху]
qint64 SdTime2x::toEpoch(qint32 tm)
  {
  return SD_TIME_2X_START + static_cast<qint64>(tm);
  }




//Convert from epoch [Преобразовать из эпохи]
qint32 SdTime2x::fromEpoch(qint64 tm)
  {
  //При отсутствии времени возвращаем ноль обозначающий тоже отсутствие времени
  if( tm == 0 )
    return 0;
  return static_cast<qint32>( tm - SD_TIME_2X_START);
  }




//Get real utc date [Получить реальную дату]
QDateTime SdTime2x::toUtc(qint32 tm)
  {
  if( tm <= 0 )
    return QDateTime();
  return QDateTime::fromSecsSinceEpoch( toEpoch(tm), Qt::UTC );
  }




//Get real local date [Получить реальную локальную дату]
QDateTime SdTime2x::toLocal(qint32 tm)
  {
  if( tm <= 0 )
    return QDateTime();
  return QDateTime::fromSecsSinceEpoch( toEpoch(tm) );
  }




//Convert from QDataTime [Преобразовать из момента]
qint32 SdTime2x::fromDateTime(const QDateTime tm)
  {
  return fromEpoch( tm.toSecsSinceEpoch() );
  }




//Convert to string representation of time
QString SdTime2x::toLocalString(qint32 tm)
  {
  return toLocal(tm).toString( QString("HH:mm dd-MM-yy") );
  }



