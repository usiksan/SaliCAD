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
#ifndef SDPVSTRATUM_H
#define SDPVSTRATUM_H

#include "SdConfig.h"
#include "SvLib/SvJsonIO.h"

#include <QString>
#include <QDataStream>

#define stmRuleHided 0x1 //!< Enable hided holes (stmInt00-stmInt01)
#define stmRuleCore  0x2 //!< Enable core holes (stmInt00-stmInt27)
#define stmRuleBlind 0x4 //!< Enable blind holes (stmTop-stmInt00)

//Stratum difference for version 1
#define stmBottomV1 0x00000002


//Tracing layer mask (stratum)
#define stmTop      0x00000001 //Top layer
#define stmInt02    0x00000002 //Internal packet 1
#define stmInt03    0x00000004
#define stmInt04    0x00000008 //Internal packet 2
#define stmInt05    0x00000010
#define stmInt06    0x00000020 //Internal packet 3
#define stmInt07    0x00000040
#define stmInt08    0x00000080 //Internal packet 4
#define stmInt09    0x00000100
#define stmInt10    0x00000200 //Internal packet 5
#define stmInt11    0x00000400
#define stmInt12    0x00000800 //Internal packet 6
#define stmInt13    0x00001000
#define stmInt14    0x00002000 //Internal packet 7
#define stmInt15    0x00004000
#define stmInt16    0x00008000 //Internal packet 8
#define stmInt17    0x00010000
#define stmInt18    0x00020000 //Internal packet 9
#define stmInt19    0x00040000
#define stmInt20    0x00080000 //Internal packet 10
#define stmInt21    0x00100000
#define stmInt22    0x00200000 //Internal packet 11
#define stmInt23    0x00400000
#define stmInt24    0x00800000 //Internal packet 12
#define stmInt25    0x01000000
#define stmInt26    0x02000000 //Internal packet 13
#define stmInt27    0x04000000
#define stmInt28    0x08000000 //Internal packet 14
#define stmInt29    0x10000000
#define stmBottom   0x20000000 //Bottom layer
#define stmThrough  0x3fffffff
#define stmEnd      0x40000000

//Defines PCB with layer number
#define stmLayer1   (stmTop)                            //!< Single layer PCB (only top layer)
#define stmLayer2   (stmTop | stmBottom)                //!< Two layer PCB (top and bottom layers)
#define stmLayer4   (stmLayer2 | stmInt02 | stmInt29)   //!< 4 layer PCB (adds two inner layers)
#define stmLayer6   (stmLayer4 | stmInt03 | stmInt04)   //!< 6 layer PCB (adds two more inner layers)
#define stmLayer8   (stmLayer6 | stmInt05 | stmInt06)   //!< 8 layer PCB configuration
#define stmLayer10  (stmLayer8 | stmInt07 | stmInt08)   //!< 10 layer PCB configuration
#define stmLayer12  (stmLayer10 | stmInt09 | stmInt10)  //!< 12 layer PCB configuration
#define stmLayer14  (stmLayer12 | stmInt11 | stmInt12)  //!< 14 layer PCB configuration
#define stmLayer16  (stmLayer14 | stmInt13 | stmInt14)  //!< 16 layer PCB configuration
#define stmLayer18  (stmLayer16 | stmInt15 | stmInt16)  //!< 18 layer PCB configuration
#define stmLayer20  (stmLayer18 | stmInt17 | stmInt18)  //!< 20 layer PCB configuration
#define stmLayer22  (stmLayer20 | stmInt19 | stmInt20)  //!< 22 layer PCB configuration
#define stmLayer24  (stmLayer22 | stmInt21 | stmInt22)  //!< 24 layer PCB configuration
#define stmLayer26  (stmLayer24 | stmInt23 | stmInt24)  //!< 26 layer PCB configuration
#define stmLayer28  (stmLayer26 | stmInt25 | stmInt26)  //!< 28 layer PCB configuration
#define stmLayer30  (stmLayer28 | stmInt27 | stmInt28)  //!< 30 layer PCB configuration (maximum)
#define stmCountMax         30                          //!< Maximum tracing layers



using SdStratumValue = quint32;


class SdPvStratum
  {
    SdStratumValue mValue;
  public:
    SdPvStratum( SdStratumValue str = stmTop ) : mValue(str) {}

    bool     operator != ( const SdPvStratum &v ) const { return mValue != v.mValue; }

    bool     isEmpty() const { return mValue == 0; }

    //!
    //! \brief stratumBuild  Build stratum from one stratum to another
    //! \param from          Source stratum
    //! \param to            Destignation stratum
    //! \param pcbLayerCount Count of pcb signal layers
    //! \param rule          Rule for building, one of stmRuleXXX
    //!
    void     stratumBuild( SdPvStratum from, SdPvStratum to, int pcbLayerCount, int rule );

    //Return true if this stratum intersect with given stratum presented with int
    //bool     isMatchPartial( SdPvStratum s ) const { return mValue & s; }
    bool     isMatchPartial( SdPvStratum s ) const { return mValue & s.mValue; }

    //bool     isMatchExact( SdPvStratum s ) const { return mValue == s; }
    bool     isMatchExact( SdPvStratum s ) const { return mValue == s.mValue; }

    void     operator |= ( SdPvStratum s ) { mValue |= s.mValue; }

    bool     isIntersect( SdPvStratum s ) const { return mValue & s.mValue; }

    SdPvStratum operator & ( SdPvStratum s ) const { return SdPvStratum( mValue & s.mValue ); }

    //!
    //! \brief json Write integer property to JSON writer
    //! \param key JSON key name
    //! \param js  JSON writer object
    //!
    void     json( const QString key, SvJsonWriter &js ) const { js.jsonHex32( key, mValue ); }

    //!
    //! \brief json Read integer property from JSON reader
    //! \param key JSON key name
    //! \param js  JSON reader object
    //!
    void     json( const QString key, const SvJsonReader &js ) { js.jsonHex32( key, mValue ); }

    //Write-read stratum
    void     json( SvJsonWriter &js ) const;
    void     json( const SvJsonReader &js );

    //Write-read as side
    void     jsonSide( SvJsonWriter &js ) const;
    void     jsonSide( const SvJsonReader &js );

    //Return true if exactly top side
    bool     isTop() const { return mValue == stmTop; }

    //Return true if exactly bottom side
    bool     isBottom() const { return mValue == stmBottom; }

    //Flip stratum stack for pcb bottom and unchange it if other
    SdPvStratum stratum( bool top = true ) const;

    //Flip stratum stack if component is bottom and unchange it if none
    SdPvStratum stratumComp( const SdPvStratum &src ) const;

    //Get first stratum from stratum stack
    SdPvStratum stratumFirst( SdPvStratum s ) const;

    //Get next stratum from stratum stack
    SdPvStratum stratumNext( SdPvStratum s ) const;

    int         getStratumIndex() const;

    void        swap( SdPvStratum &other ) { qSwap( mValue, other.mValue ); }

    //Get stratum index of first stratum in the stack
    static int         stratumIndex( SdPvStratum stratum );

    //Build stratum stack with stratum count
    static SdPvStratum stratumStack( int stratumCount );

    inline static SdPvStratum stratumTop() { return SdPvStratum(stmTop); }

    inline static SdPvStratum stratumBot() { return SdPvStratum(stmBottom); }

    inline static SdPvStratum stratumThrough() { return SdPvStratum(stmThrough); }

    friend QDataStream &operator << ( QDataStream &os, SdPvStratum stratum );
    friend QDataStream &operator >> ( QDataStream &is, SdPvStratum &stratum );
  };

inline QDataStream &operator << ( QDataStream &os, SdPvStratum stratum )
  {
  os << stratum.mValue;
  return os;
  }

inline QDataStream &operator >> ( QDataStream &is, SdPvStratum &stratum )
  {
  is >> stratum.mValue;
  return is;
  }

#endif // SDPVSTRATUM_H
