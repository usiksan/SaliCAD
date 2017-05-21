/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdGraphPartImp.h"
#include "SdPItemPlate.h"
#include "SdGraphPartPin.h"


//====================================================================================
//Pin for part implementation
SdPartImpPin::SdPartImpPin() :
  mPin(nullptr),
  mCom(false),       //Pin to wire flag connection
  mPadStack(nullptr)  //Pad stack
  {
  }




void SdPartImpPin::operator =(SdPartImpPin &pin)
  {
  mPin       = pin.mPin;
  mPinNumber = pin.mPinNumber; //Part pin number
  mPinName   = pin.mPinName;   //Part pin name
  mWireName  = pin.mWireName;  //Name of net pin conneted to
  mPosition  = pin.mPosition;  //Pin position in plate context
  mCom       = pin.mCom;       //Pin to wire flag connection
  mPadStack  = pin.mPadStack;  //Pad stack
  }




void SdPartImpPin::setConnection(const QString wireName, bool com)
  {
  mCom = com;
  if( mCom )
    mWireName = wireName;
  }




void SdPartImpPin::draw(SdContext *dc)
  {
  mPin->drawImp( dc, mPinName, mCom );
  }



//====================================================================================
//Part implementation
SdGraphPartImp::SdGraphPartImp()
  {

  }




void SdGraphPartImp::pinConnectionSet(int pinIndex, const QString wireName, bool com)
  {
  //If no pinIndex then doing nothink
  if( pinIndex < 0 ) return;
  Q_ASSERT( pinIndex >= 0 && pinIndex < mPins.count() );
  mPins[pinIndex].setConnection( wireName, com );
  }





SdPItemPlate *SdGraphPartImp::getPlate() const
  {
  return dynamic_cast<SdPItemPlate*>( getParent() );
  }




//Return pin index of pinNumber
int SdGraphPartImp::getPinIndex(const QString pinNumber) const
  {
  for( int index = 0; index < mPins.count(); index++ )
    if( mPins[index].mPinNumber == pinNumber ) return index;
  //No pin with that pinNumber
  return -1;
  }




//Check if there free section slot
bool SdGraphPartImp::isSectionFree(SdPItemPart *part, SdPItemSymbol *comp, SdPItemSymbol *section)
  {
  if( mPart != part || mComp != comp )
    return false;
  for( SdPartImpSection &sec : mSections )
    if( sec.mSymbol == section && sec.mSymImp == nullptr )
      return true;
  return false;
  }

