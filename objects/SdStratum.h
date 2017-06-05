/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing layers order and contens
*/
#ifndef SDSTRATUM_H
#define SDSTRATUM_H

#include "SdConfig.h"
#include <QJsonObject>

#define stmTop      0x000000000000001l
#define stmBottom   0x800000000000000l
#define stmThrow    0xfffffffffffffffl

class SdStratum
  {
    qint64 mStratum;
  public:
    SdStratum();
    SdStratum( qint64 str );

    bool match( SdStratum s ) const { return (mStratum & s.mStratum) != 0; }

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject &obj );
  };

#endif // SDSTRATUM_H
