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
#ifndef SDPROPSTRATUM_H
#define SDPROPSTRATUM_H

#include "SdConfig.h"
#include "SdPropInt.h"
#include "SdStratum.h"


#define stmRuleHided 0x1 //!< Enable hided holes (stmInt00-stmInt01)
#define stmRuleCore  0x2 //!< Enable core holes (stmInt00-stmInt27)
#define stmRuleBlind 0x4 //!< Enable blind holes (stmTop-stmInt00)

class SdJsonReader;

struct SdPropStratum
  {
    SdStratum mValue;

    SdPropStratum();
    SdPropStratum( int str );

    //!
    //! \brief stratumBuild  Build stratum from one stratum to another
    //! \param from          Source stratum
    //! \param to            Destignation stratum
    //! \param pcbLayerCount Count of pcb signal layers
    //! \param rule          Rule for building, one of stmRuleXXX
    //!
    void     stratumBuild( SdPropStratum from, SdPropStratum to, int pcbLayerCount, int rule );

    //Return true if this stratum intersect with given stratum presented with int
    bool     operator & ( int s ) const { return mValue & s; }

    void     operator |= ( SdPropStratum s ) { mValue |= s.mValue; }

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
    void     json( const SdJsonReader &js );

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
    int      stratumComp( const SdPropStratum &src ) const;

    //Get first stratum from stratum stack
    int      stratumFirst( SdPropStratum s ) const;

    //Get next stratum from stratum stack
    int      stratumNext( SdPropStratum s ) const;

    int      getStratumIndex() const;

    void     swap( SdPropStratum &other ) { qSwap( mValue, other.mValue ); }

    //Get stratum index of first stratum in the stack
    static int stratumIndex( int stratum );

    //Build stratum stack with stratum count
    static int stratumStack( int stratumCount );

  };

#endif // SDPROPSTRATUM_H
