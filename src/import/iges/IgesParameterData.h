#ifndef IGESPARAMETERDATA_H
#define IGESPARAMETERDATA_H

#include <QByteArray>
#include <QList>

class IgesParameterData
  {
    QByteArray mData;
  public:
    IgesParameterData();

    void appendData( const QByteArray &ar ) { mData += ar; }
  };

using IgesParameterDataList = QList<IgesParameterData>;

#endif // IGESPARAMETERDATA_H
