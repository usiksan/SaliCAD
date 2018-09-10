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
#define stmInt08    0x00000400
#define stmInt09    0x00000800
#define stmInt10    0x00001000
#define stmInt11    0x00002000
#define stmInt12    0x00004000
#define stmInt13    0x00008000
#define stmInt14    0x00010000
#define stmInt15    0x00020000
#define stmInt16    0x00040000
#define stmInt17    0x00080000
#define stmInt18    0x00100000
#define stmInt19    0x00200000
#define stmInt20    0x00400000
#define stmInt21    0x00800000
#define stmInt22    0x01000000
#define stmInt23    0x02000000
#define stmInt24    0x04000000
#define stmInt25    0x08000000
#define stmInt26    0x10000000
#define stmInt27    0x20000000
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
