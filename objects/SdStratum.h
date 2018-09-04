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

//Tracing layer mask (stratum)
#define stmTop      0x00000001
#define stmBottom   0x00000002
#define stmThrough  0x3fffffff
#define stmInt00    0x00000004
#define stmInt01    0x00000008
#define stmInt02    0x00000010
#define stmInt03    0x00000020
#define stmInt04    0x00000040
#define stmInt05    0x00000080
#define stmInt06    0x00000100
#define stmInt07    0x00000200
#define stmEnd      0x40000000

#define stmCountMax         30 //Maximum tracing layers

class SdStratum : public SdPropInt
  {
  public:
    SdStratum();
    SdStratum( int str );

    //Return true if this stratum intersect with given stratum presented with int
    bool     operator & ( int s ) const { return mValue > 0 && s > 0 && (mValue & s); }

    //Return true if this stratum intersect with given stratum
    bool     match( SdStratum s ) const { return mValue > 0 && s.mValue > 0 && (mValue & s.mValue) != 0; }

    //Write-read stratum
    void     writeStratum(QJsonObject &obj) const;
    void     readStratum(const QJsonObject &obj);

    //Return true if exactly top side
    bool     isTop() const { return mValue == stmTop; }

    //Return true if exactly bottom side
    bool     isBottom() const { return mValue == stmBottom; }

    //Flip stratum stack for pcb bottom and unchange it if other
    int      stratum( bool top = true ) const;

    //Flip stratum stack if component is bottom and unchange it if none
    int      stratumComp( const SdStratum &src ) const;

    //Get first stratum from stratum stack
    int      stratumFirst( SdStratum s ) const;

    //Get next stratum from stratum stack
    int      stratumNext( SdStratum s ) const;

    int      getStratumIndex() const;

    //Get stratum index of first stratum in the stack
    static int stratumIndex( int stratum );

  };

#endif // SDSTRATUM_H
