/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing layers identify
  Supporting max 30 layers
*/
#ifndef SDSTRATUM_H
#define SDSTRATUM_H

#include "SdConfig.h"
#include "SdPropInt.h"
#include <QJsonObject>

#define stmTop    0x00000001
#define stmBottom 0x00000002
#define stmThrow  0x3fffffff
#define stmInt00  0x00000004
#define stmInt01  0x00000008
#define stmInt02  0x00000010
#define stmInt03  0x00000020

class SdStratum : public SdPropInt
  {
  public:
    SdStratum();
    SdStratum( int str );

    bool operator & ( int s ) const { return mValue > 0 && (mValue & s); }

    bool match( SdStratum s ) const { return mValue > 0 && s.mValue > 0 && (mValue & s.mValue) != 0; }

    void writeStratum(QJsonObject &obj) const;
    void readStratum(const QJsonObject &obj);

    bool isTop() const { return mValue == stmTop; }

    bool isBottom() const { return mValue == stmBottom; }

    int  stratum( bool top = true ) const;

    int  stratumComp( const SdStratum &src ) const;

    static int stratumIndex( int stratum );

  };

#endif // SDSTRATUM_H
