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
#include "SdContainerSheetNet.h"
#include "SdPItemSheet.h"

//====================================================================================
//Pin for symbol implementation

SdSymImpPin::SdSymImpPin() :
  mPin(0),       //Pin
  mCom(false)    //State of pin to net connectivity
  {

  }



void SdSymImpPin::operator =(const SdSymImpPin &pin)
  {
  mPin       = pin.mPin;       //Pin
  mPinNumber = pin.mPinNumber; //Pin number in part
  mPosition  = pin.mPosition;  //Pin position in sheet context
  mWireName  = pin.mWireName;  //Net, which pin connected to
  mCom       = pin.mCom;       //State of pin to net connectivity
  }



void SdSymImpPin::draw(SdContext *dc)
  {
  mPin->drawImp( dc, mPinNumber, mCom );
  }







bool SdSymImpPin::isCanConnect(SdPoint a, SdPoint b) const
  {
  return !mCom && mPosition.isOnSegment( a, b );
  }




bool SdSymImpPin::isCanDisconnect(SdPoint a, SdPoint b, const QString wireName) const
  {
  return mCom && mWireName == wireName && mPosition.isOnSegment( a, b );
  }




void SdSymImpPin::prepareMove(SdPItemSheet *sheet, SdSelector *selector)
  {
  if( mCom ) {
    SdContainerSheetNet *net = sheet->netGet( mWireName );
    Q_ASSERT( net != nullptr );
    net->accumLinked( mPosition, mPosition, selector );
    }
  }



QJsonObject SdSymImpPin::toJson( const QString pinName ) const
  {
  QJsonObject obj;
  obj.insert( QStringLiteral("Name"), pinName );
  SdObject::writePtr( mPin, QStringLiteral("Pin"), obj );
  obj.insert( QStringLiteral("Number"), mPinNumber ); //Pin number in part
  mPosition.write( QStringLiteral("Pos"), obj );      //Pin position in sheet context
  obj.insert( QStringLiteral("Wire"), mWireName );    //Net, which pin connected to
  obj.insert( QStringLiteral("Com"), mCom );          //State of pin to net connectivity
  return obj;
  }




QString SdSymImpPin::fromJson(SdObjectMap *map, const QJsonObject obj)
  {
  mPin = dynamic_cast<SdGraphSymPin*>( SdObject::readPtr( QStringLiteral("Pin"), map, obj ) );
  mPinNumber = obj.value( QStringLiteral("Number") ).toString();
  mPosition.read( QStringLiteral("Pos"), obj );
  mWireName = obj.value( QStringLiteral("Wire") ).toString();
  mCom = obj.value( QStringLiteral("Com") ).toBool();
  return obj.value( QStringLiteral("Name") ).toString();
  }




