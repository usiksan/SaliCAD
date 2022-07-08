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
#ifndef SVTIME2X_H
#define SVTIME2X_H

#include <QDateTime>

//The moment of the beginning of the countdown of time intervals from the beginning of 2018 [Момент начала отсчета временных интервалов от начала 2018 года]
#define SV_TIME_2X_START 1514764800l

class SvTime2x
  {
  public:
    //!
    //! \brief current Return current global moment [Текущий глобальный момент]
    //! \return        Current global moment
    //!
    static qint32    current();

    //Deal with epoch time [Взаимодействие со временем относительно эпохи]

    //!
    //! \brief toEpoch Convert to epoch time [Преобразовать в эпоху]
    //! \param tm      Time to convert
    //! \return        Epoch time for time tm
    //!
    static qint64    toEpoch( qint32 tm );

    //!
    //! \brief fromEpoch Convert from epoch time [Преобразовать из эпохи]
    //! \param tm        Epoch time to convert
    //! \return          Time2x time for epoch time tm
    //!
    static qint32    fromEpoch( qint64 tm );


    //!
    //! \brief toUtc Return universal date for time tm [Получить реальную дату]
    //! \param tm    Time to retrive utc date
    //! \return      UTC date for time tm
    //!
    static QDateTime toUtc( qint32 tm );

    //!
    //! \brief toLocal Return local date for time tm [Получить реальную локальную дату]
    //! \param tm      Time to retrive local date
    //! \return        Local date for time tm
    //!
    static QDateTime toLocal( qint32 tm );

    //!
    //! \brief fromDateTime Convert to Time2x from dateTime tm [Преобразовать из момента]
    //! \param tm           DateTime to convert into Time2x
    //! \return             Time2x for DateTime tm
    //!
    static qint32    fromDateTime( const QDateTime tm );

    //!
    //! \brief toLocalString Convert to string representation of time
    //! \param tm            Time2x to convert
    //! \return              String representation of Time2x
    //!
    static QString   toLocalString(qint32 tm);

  };

#endif // SVTIME2X_H
