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

SdGraphPartImp::SdGraphPartImp(SdPoint org, SdPItemPart *part, SdPItemSymbol *comp)
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



//Get wire name pin with pinIndex connected to
QString SdGraphPartImp::pinWireName(int pinIndex) const
  {
  if( pinIndex < 0 ) return QString();
  Q_ASSERT( pinIndex < mPins.count() );
  return mPins[pinIndex].mWireName;
  }




//Return if pin with pinIndex connected to any wire or not
bool SdGraphPartImp::isPinConnected(int pinIndex) const
  {
  if( pinIndex < 0 ) return false;
  Q_ASSERT( pinIndex < mPins.count() );
  return mPins[pinIndex].mCom;
  }




//Return pin index of pinNumber
int SdGraphPartImp::getPinIndex(const QString pinNumber) const
  {
  for( int index = 0; index < mPins.count(); index++ )
    if( mPins[index].mPinNumber == pinNumber ) return index;
  //No pin with that pinNumber
  return -1;
  }




void SdGraphPartImp::setLinkSection(int section, SdGraphSymImp *symImp)
  {
  if( section >= 0 && section < mSections.count() ) {
    mSections[section].mSymImp = symImp;
    }
  }




//Check if there free section slot
bool SdGraphPartImp::isSectionFree( int *section, SdPItemPart *part, SdPItemSymbol *comp, SdPItemSymbol *sym)
  {
  if( mPart != part || mComp != comp )
    return false;
  for( int index = 0; index < mSections.count(); index++ )
    if( mSections[index].isFree(sym) ) {
      *section = index;
      return true;
      }
  return false;
  }



void SdGraphPartImp::attach(SdUndo *undo)
    {
    }

void SdGraphPartImp::detach(SdUndo *undo)
    {
    }

void SdGraphPartImp::cloneFrom(const SdObject *src)
    {
    }

void SdGraphPartImp::writeObject(QJsonObject &obj) const
    {
    }

void SdGraphPartImp::readObject(SdObjectMap *map, const QJsonObject obj)
    {
    }

void SdGraphPartImp::saveState(SdUndo *undo)
    {
    }

void SdGraphPartImp::moveComplete(SdPoint grid, SdUndo *undo)
    {
    }

void SdGraphPartImp::move(SdPoint offset)
    {
    }

void SdGraphPartImp::rotate(SdPoint center, SdAngle angle)
    {
    }

void SdGraphPartImp::mirror(SdPoint a, SdPoint b)
    {
    }

void SdGraphPartImp::setProp(SdProp &prop)
    {
    }

void SdGraphPartImp::getProp(SdProp &prop)
    {
    }

void SdGraphPartImp::selectByPoint(const SdPoint p, SdSelector *selector)
    {
    }

void SdGraphPartImp::selectByRect(const SdRect &r, SdSelector *selector)
    {
    }

void SdGraphPartImp::select(SdSelector *selector)
    {
    }

void SdGraphPartImp::prepareMove()
    {
    }

bool SdGraphPartImp::canHideLayer(SdLayer *layer)
    {
    }

bool SdGraphPartImp::isVisible()
    {
    }

SdRect SdGraphPartImp::getOverRect() const
    {
    }

void SdGraphPartImp::draw(SdContext *dc)
    {
    }

int SdGraphPartImp::behindCursor(SdPoint p)
    {
    }

bool SdGraphPartImp::getInfo(SdPoint p, QString &info, bool extInfo)
    {
    }

bool SdGraphPartImp::snapPoint(SdSnapInfo *snap)
    {
    }
