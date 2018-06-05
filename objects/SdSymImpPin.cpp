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

//====================================================================================
//Pin for symbol implementation

SdSymImpPin::SdSymImpPin() :
  mPin(nullptr)       //Pin
  {
  }



void SdSymImpPin::operator =(const SdSymImpPin &pin)
  {
  mPin       = pin.mPin;       //Pin
  mPinNumber = pin.mPinNumber; //Pin number in part
  mPosition  = pin.mPosition;  //Pin position in sheet context
  mNetName  = pin.mNetName;  //Net, which pin connected to
  }



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



QJsonObject SdSymImpPin::toJson( const QString pinName ) const
  {
  QJsonObject obj;
  obj.insert( QStringLiteral("Name"), pinName );
  SdObject::writePtr( mPin, QStringLiteral("Pin"), obj );
  obj.insert( QStringLiteral("Number"), mPinNumber ); //Pin number in part
  mPosition.write( QStringLiteral("Pos"), obj );      //Pin position in sheet context
  obj.insert( QStringLiteral("Wire"), mNetName );    //Net, which pin connected to
  return obj;
  }




QString SdSymImpPin::fromJson(SdObjectMap *map, const QJsonObject obj)
  {
  mPin = dynamic_cast<SdGraphSymPin*>( SdObject::readPtr( QStringLiteral("Pin"), map, obj ) );
  mPinNumber = obj.value( QStringLiteral("Number") ).toString();
  mPosition.read( QStringLiteral("Pos"), obj );
  mNetName = obj.value( QStringLiteral("Wire") ).toString();
  return obj.value( QStringLiteral("Name") ).toString();
  }




