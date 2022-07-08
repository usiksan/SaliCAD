/*
  Project  "SvLib - common library"
  Author
    Alexander Sibilev
  Internet
    www.saliLab.com

  Description
    Common short time

    All time in seconds 32-bit from begin of year 2018.
*/
#include "SvTime2x.h"
#include <QDebug>



//!
//! \brief current Return current global moment [Текущий глобальный момент]
//! \return        Current global moment
//!
qint32 SvTime2x::current()
  {
  return fromEpoch( QDateTime::currentSecsSinceEpoch() );
  }



//!
//! \brief toEpoch Convert to epoch time [Преобразовать в эпоху]
//! \param tm      Time to convert
//! \return        Epoch time for time tm
//!
qint64 SvTime2x::toEpoch(qint32 tm)
  {
  return SV_TIME_2X_START + static_cast<qint64>(tm);
  }




//!
//! \brief fromEpoch Convert from epoch time [Преобразовать из эпохи]
//! \param tm        Epoch time to convert
//! \return          Time2x time for epoch time tm
//!
qint32 SvTime2x::fromEpoch(qint64 tm)
  {
  //При отсутствии времени возвращаем ноль обозначающий тоже отсутствие времени
  if( tm == 0 )
    return 0;
  return static_cast<qint32>( tm - SV_TIME_2X_START);
  }




//!
//! \brief toUtc Return universal date for time tm [Получить реальную дату]
//! \param tm    Time to retrive utc date
//! \return      UTC date for time tm
//!
QDateTime SvTime2x::toUtc(qint32 tm)
  {
  if( tm <= 0 )
    return QDateTime();
  return QDateTime::fromSecsSinceEpoch( toEpoch(tm), Qt::UTC );
  }




//!
//! \brief toLocal Return local date for time tm [Получить реальную локальную дату]
//! \param tm      Time to retrive local date
//! \return        Local date for time tm
//!
QDateTime SvTime2x::toLocal(qint32 tm)
  {
  if( tm <= 0 )
    return QDateTime();
  return QDateTime::fromSecsSinceEpoch( toEpoch(tm) );
  }




//!
//! \brief fromDateTime Convert to Time2x from dateTime tm [Преобразовать из момента]
//! \param tm           DateTime to convert into Time2x
//! \return             Time2x for DateTime tm
//!
qint32 SvTime2x::fromDateTime(const QDateTime tm)
  {
  return fromEpoch( tm.toSecsSinceEpoch() );
  }




//!
//! \brief toLocalString Convert to string representation of time
//! \param tm            Time2x to convert
//! \return              String representation of Time2x
//!
QString SvTime2x::toLocalString(qint32 tm)
  {
  return toLocal(tm).toString( QString("HH:mm dd-MM-yy") );
  }



