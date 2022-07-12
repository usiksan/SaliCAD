#ifndef IGESSCANER_H
#define IGESSCANER_H

#include <QString>

class IgesScaner
  {
  public:
    static int    scanInt( const QByteArray &line, int &index );
    static double scanReal( const QByteArray &line, int &index );
    static QString scanString( const QByteArray &line, int &index );
  };

#endif // IGESSCANER_H
