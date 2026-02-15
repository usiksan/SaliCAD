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
#define stmLayer4   (stmLayer2 | stmInt02 | stmInt03)   //!< 4 layer PCB (adds two inner layers)
#define stmLayer6   (stmLayer4 | stmInt04 | stmInt05)   //!< 6 layer PCB (adds two more inner layers)
#define stmLayer8   (stmLayer6 | stmInt06 | stmInt07)   //!< 8 layer PCB configuration
#define stmLayer10  (stmLayer8 | stmInt08 | stmInt09)   //!< 10 layer PCB configuration
#define stmLayer12  (stmLayer10 | stmInt10 | stmInt11)  //!< 12 layer PCB configuration
#define stmLayer14  (stmLayer12 | stmInt12 | stmInt13)  //!< 14 layer PCB configuration
#define stmLayer16  (stmLayer14 | stmInt14 | stmInt15)  //!< 16 layer PCB configuration
#define stmLayer18  (stmLayer16 | stmInt16 | stmInt17)  //!< 18 layer PCB configuration
#define stmLayer20  (stmLayer18 | stmInt18 | stmInt19)  //!< 20 layer PCB configuration
#define stmLayer22  (stmLayer20 | stmInt20 | stmInt21)  //!< 22 layer PCB configuration
#define stmLayer24  (stmLayer22 | stmInt22 | stmInt23)  //!< 24 layer PCB configuration
#define stmLayer26  (stmLayer24 | stmInt24 | stmInt25)  //!< 26 layer PCB configuration
#define stmLayer28  (stmLayer26 | stmInt26 | stmInt27)  //!< 28 layer PCB configuration
#define stmLayer30  (stmLayer28 | stmInt28 | stmInt29)  //!< 30 layer PCB configuration (maximum)
#define stmCountMax         30                          //!< Maximum tracing layers

#define stmRuleHided 0x1 //!< Enable hided holes (stmInt00-stmInt01)
#define stmRuleCore  0x2 //!< Enable core holes (stmInt00-stmInt27)
#define stmRuleBlind 0x4 //!< Enable blind holes (stmTop-stmInt00)

class SdJsonReader;

class SdStratum : public SdPropInt
  {
  public:
    SdStratum();
    SdStratum( int str );

    //!
    //! \brief stratumBuild  Build stratum from one stratum to another
    //! \param from          Source stratum
    //! \param to            Destignation stratum
    //! \param pcbLayerCount Count of pcb signal layers
    //! \param rule          Rule for building, one of stmRuleXXX
    //!
    void     stratumBuild( SdStratum from, SdStratum to, int pcbLayerCount, int rule );

    //Return true if this stratum intersect with given stratum presented with int
    bool     operator & ( int s ) const { return mValue > 0 && s > 0 && (mValue & s); }

    void     operator |= ( SdStratum s ) { mValue |= s.mValue; }

    //Return true if this stratum intersect with given stratum
    bool     match( const SdStratum &s ) const { return mValue > 0 && s.mValue > 0 && (mValue & s.mValue) != 0; }

    //Write-read stratum
    void     jsonStratum( SvJsonWriter &js ) const;
    void     jsonStratum( const SdJsonReader &js );

    //Write-read as side
    void     jsonSide( SvJsonWriter &js ) const;
    void     jsonSide( const SdJsonReader &js );

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

    //Build stratum stack with stratum count
    static int stratumStack( int stratumCount );

  };

#endif // SDSTRATUM_H
