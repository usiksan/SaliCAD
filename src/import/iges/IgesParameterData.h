#ifndef IGESPARAMETERDATA_H
#define IGESPARAMETERDATA_H

#include <QByteArray>
#include <QMap>

class IgesParameterData
  {
    QByteArray mData;
  public:
    IgesParameterData();

    void appendData( const QByteArray &ar ) { mData += ar; }
  };

using IgesParameterDataMap = QMap<int,QByteArray>;

#endif // IGESPARAMETERDATA_H
