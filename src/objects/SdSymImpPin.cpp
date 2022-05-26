/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Pin for symbol implementation
*/
#include "SdSymImpPin.h"
#include "SdGraphSymPin.h"
#include "SdPItemSheet.h"
#include "SdGraphNetWire.h"
#include "SdJsonIO.h"

//====================================================================================
//Pin for symbol implementation

SdSymImpPin::SdSymImpPin() :
  mPin(nullptr)       //Pin
  {
  }



//SdSymImpPin &SdSymImpPin::operator =(const SdSymImpPin &pin)
//  {
//  mPin       = pin.mPin;       //Pin
//  mPinNumber = pin.mPinNumber; //Pin number in part
//  mPosition  = pin.mPosition;  //Pin position in sheet context
//  mNetName   = pin.mNetName;  //Net, which pin connected to
//  return *this;
//  }



void SdSymImpPin::draw(SdContext *dc) const
  {
  mPin->drawImp( dc, mPinNumber, isConnected() );
  }







bool SdSymImpPin::isCanConnect(SdPoint a, SdPoint b) const
  {
  return !isConnected() && mPosition.isOnSegment( a, b );
  }




bool SdSymImpPin::isCanDisconnect(SdPoint a, SdPoint b, const QString wireName) const
  {
  return mNetName == wireName && mPosition.isOnSegment( a, b );
  }




void SdSymImpPin::prepareMove(SdPItemSheet *sheet, SdSelector *selector, SdUndo *undo )
  {
  if( isConnected() )
    //Scan all segments to find connected to pin
    sheet->accumLinked( mPosition, mPosition, mNetName, selector, undo );
  }






QString SdSymImpPin::fromJson(const SdJsonReader &js)
  {
  json( js );
  return js.object().value( QStringLiteral("Name") ).toString();
  }


//!
//! \brief json Function to write object content into json writer
//! \param js   Json writer
//!
void SdSymImpPin::json(SdJsonWriter &js) const
  {
  js.jsonObjectPtr( QStringLiteral("Pin"), mPin );
  //Pin number in part
  js.jsonString( QStringLiteral("Number"), mPinNumber );
  //Pin position in sheet context
  js.jsonPoint( QStringLiteral("Pos"), mPosition );
  //Net, which pin connected to
  js.jsonString( QStringLiteral("Wire"), mNetName );
  }




//!
//! \brief json Function to read object content from json reader
//! \param js   Json reader
//!
void SdSymImpPin::json(const SdJsonReader &js)
  {
  js.jsonObjectPtr( QStringLiteral("Pin"), mPin );
  //Pin number in part
  js.jsonString( QStringLiteral("Number"), mPinNumber );
  //Pin position in sheet context
  js.jsonPoint( QStringLiteral("Pos"), mPosition );
  //Net, which pin connected to
  js.jsonString( QStringLiteral("Wire"), mNetName );
  }




