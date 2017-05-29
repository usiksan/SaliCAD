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
#include "SdGraphIdent.h"
#include "SdGraphSymImp.h"
#include "SdConverterImplement.h"
#include "SdPItemPart.h"
#include "SdPItemSymbol.h"
#include "SdProject.h"
#include "SdContext.h"
#include "SdSelector.h"


//====================================================================================
//Pin for part implementation
SdPartImpPin::SdPartImpPin() :
  mPin(nullptr),
  mCom(false),       //Pin to wire flag connection
  mPadStack(nullptr)  //Pad stack
  {
  }




void SdPartImpPin::operator =(const SdPartImpPin &pin)
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
SdGraphPartImp::SdGraphPartImp() :
  SdGraph(),
  mPart(nullptr),
  mComponent(nullptr)
  {

  }

SdGraphPartImp::SdGraphPartImp(SdPoint org, SdPropPartImp *prp, SdPItemPart *part, SdPItemSymbol *comp) :
  SdGraph(),
  mLogNumber(0),   //Logical part number (from 1)
  mOrigin(org),    //Position of Implement
  mPart(part),     //Part for this implementation
  mComponent(comp)      //Component
  {
  //Implement properties
  mProp = *prp;
  if( part ) {
    SdConverterImplement imp( mOrigin, part->mOrigin, mProp.mAngle.getValue(), mProp.mMirror.getValue(), mProp.mSide.getValue() );
    QTransform t( imp.getMatrix() );
    mOverRect.set( t.mapRect(part->getOverRect()) );//Over rect
    mPrefix = part->getIdent()->getText();          //Part identificator prefix
    mIdentProp = part->getIdent()->getPropText();   //Part identificator text properties
    mIdentOrigin = part->getIdent()->getOrigin();   //Part identificator position in symbol context
    mIdentPos = t.map( mIdentOrigin );    //Part identificator position in sheet context
    mIdentRect.set( t.mapRect( part->getIdent()->getOverRect() )  );   //Part identificator over rect
    }
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



//Get full visual ident of part aka D4 or R45
QString SdGraphPartImp::getIdent() const
  {
  return mPrefix + QString::number(mLogNumber);
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
  if( mPart != part || mComponent != comp )
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
  SdPItemPlate *plate = getPlate();
  Q_ASSERT( plate != nullptr );
  SdProject *prj = plate->getProject();
  Q_ASSERT( prj != nullptr );
  //Realloc objects for this project
  mComponent = dynamic_cast<SdPItemSymbol*>( prj->getProjectsItem(mComponent) );  //Object contains section information, pin assotiation info. May be same as mSymbol.
  mPart = dynamic_cast<SdPItemPart*>( prj->getProjectsItem(mPart) );

  //Fill pin table
  mPins.clear();
  if( mPart ) {
    mPart->forEach( dctPartPin, [this, plate] (SdObject *obj) -> bool {
      SdGraphPartPin *pin = dynamic_cast<SdGraphPartPin*>(obj);
      Q_ASSERT( pin != nullptr );

      //Create implement pin
      SdPartImpPin impPin;
      impPin.mPin       = pin;
      impPin.mPinNumber = pin->getPinNumber();
      impPin.mPadStack  = plate->getPad( pin->getPinType() );

      //Add pin to table
      mPins.append( impPin );
      return true;
      });
    }

  //Section table
  mSections.clear();
  if( mComponent ) {
    int sectionCount = mComponent->getSectionCount();
    for( int i = 0; i < sectionCount; i++ ) {
      SdPartImpSection s;
      SdPItemSymbol *sym = mComponent->extractSymbolFromFactory( i, false, nullptr );
      s.mSymbol = dynamic_cast<SdPItemSymbol*>( prj->getProjectsItem(sym) );
      if( sym )
        delete sym;
      mSections.append( s );
      }
    }
  }





void SdGraphPartImp::detach(SdUndo *undo)
  {
  //Unlink all sections
  for( SdPartImpSection &s : mSections ) {
    if( s.mSymImp )
      s.mSymImp->unLinkPartImp( undo );
    }
  }




void SdGraphPartImp::cloneFrom(const SdObject *src)
  {
  SdGraph::cloneFrom( src );
  const SdGraphPartImp *imp = dynamic_cast<const SdGraphPartImp*>( src );
  Q_ASSERT(imp != nullptr);
  mLogNumber   = imp->mLogNumber;   //Logical part number (from 1)
  mOrigin      = imp->mOrigin;      //Position of Implement
  mProp        = imp->mProp;        //Implement properties
  mOverRect    = imp->mOverRect;    //Over rect
  mPrefix      = imp->mPrefix;      //Part identificator prefix
  mIdentProp   = imp->mIdentProp;   //Part identificator text properties
  mIdentOrigin = imp->mIdentOrigin; //Part identificator position in symbol context
  mIdentPos    = imp->mIdentPos;    //Part identificator position in sheet context
  mIdentRect   = imp->mIdentRect;   //Part identificator over rect

//  SdPartImpPinTable      mPins;
//  SdPartImpSectionTable  mSections;
  mPart        = imp->mPart;        //Part for this implementation
  mComponent   = imp->mComponent;   //Component
  }





void SdGraphPartImp::writeObject(QJsonObject &obj) const
  {
  SdGraph::writeObject( obj );
  obj.insert( QStringLiteral("LogNum"), mLogNumber );
  mOrigin.write( QStringLiteral("Org"), obj );
  mProp.write( obj );
  mOverRect.write( QStringLiteral("Over"), obj );
  obj.insert( QStringLiteral("Prefix"), mPrefix );
  mIdentProp.write( QStringLiteral("IdentProp"), obj );
  mIdentOrigin.write( QStringLiteral("IdentOrg"), obj );
  mIdentPos.write( QStringLiteral("IdentPos"), obj );
  mIdentRect.write( QStringLiteral("IdentRect"), obj );
  writePtr( mPart, QStringLiteral("Part"), obj );
  writePtr( mComponent, QStringLiteral("Comp"), obj );
  }





void SdGraphPartImp::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mLogNumber = obj.value( QStringLiteral("LogNum") ).toInt();
  mOrigin.read( QStringLiteral("Org"), obj );
  mProp.read( obj );
  mOverRect.read( QStringLiteral("Over"), obj );
  mPrefix = obj.value( QStringLiteral("Prefix") ).toString();
  mIdentProp.read( QStringLiteral("IdentProp"), obj );
  mIdentOrigin.read( QStringLiteral("IdentOrg"), obj );
  mIdentPos.read( QStringLiteral("IdentPos"), obj );
  mIdentRect.read( QStringLiteral("IdentRect"), obj );
  mPart = dynamic_cast<SdPItemPart*>( readPtr( QStringLiteral("Part"), map, obj )  );
  mComponent = dynamic_cast<SdPItemSymbol*>( readPtr( QStringLiteral("Comp"), map, obj )  );
  }




void SdGraphPartImp::saveState(SdUndo *undo)
  {
  undo->partImp( &mOrigin, &mProp, &mLogNumber, &mOverRect, &mPrefix, &mIdentProp, &mIdentOrigin, &mIdentPos, &mIdentRect );
  }





void SdGraphPartImp::move(SdPoint offset)
  {
  mOrigin.move( offset );
  updatePinsPositions();
  }




void SdGraphPartImp::rotate(SdPoint center, SdAngle angle)
  {
  mOrigin.rotate( center, angle );
  mProp.mAngle += angle;
  updatePinsPositions();
  }




void SdGraphPartImp::mirror(SdPoint a, SdPoint b)
  {
  mOrigin.mirror( a, b );
  updatePinsPositions();
  }




void SdGraphPartImp::setProp(SdProp &prop)
  {
  mProp = prop.mPartImpProp;
  }




void SdGraphPartImp::getProp(SdProp &prop)
  {
  prop.mPartImpProp.append( mProp );
  }




void SdGraphPartImp::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( !getSelector() && mOverRect.isPointInside(p) )
    selector->insert( this );
  }




void SdGraphPartImp::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( !getSelector() && mOverRect.isAccross(r) )
    selector->insert( this );
  }




void SdGraphPartImp::select(SdSelector *selector)
  {
  selector->insert( this );
  }





bool SdGraphPartImp::canHideLayer(SdLayer *layer)
  {
  return mIdentProp.mLayer.canHide( layer );
  }



bool SdGraphPartImp::isVisible()
  {
  return true;
  }




SdRect SdGraphPartImp::getOverRect() const
  {
  return mOverRect;
  }




void SdGraphPartImp::draw(SdContext *dc)
  {
  //Draw ident in plate context
  dc->text( mIdentPos, mIdentRect, getIdent(), mIdentProp );
  //Convertor for symbol implementation
  SdConverterImplement imp( mOrigin, mPart->mOrigin, mProp.mAngle.getValue(), mProp.mMirror.getValue(), mProp.mSide.getValue() );
  dc->setConverter( &imp );

  //Draw symbol except ident and pins
  mPart->forEach( dctAll & ~(dctPartPin | dctIdent), [dc] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph )
      graph->draw( dc );
    return true;
    });

  //Draw pins
  for( int i = 0; i < mPins.count(); i++ )
    mPins[i].draw( dc );
  }




int SdGraphPartImp::behindCursor(SdPoint p)
  {
  if( mOverRect.isPointInside(p) )
    return getSelector() ? SEL_ELEM : UNSEL_ELEM;
  return 0;
  }




bool SdGraphPartImp::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  //TODO getInfo partImp
  return false;
  }




bool SdGraphPartImp::snapPoint(SdSnapInfo *snap)
  {
  //TODO snap partImp
  return false;
  }




void SdGraphPartImp::updatePinsPositions()
  {
  SdConverterImplement impl( mOrigin, mPart->mOrigin, mProp.mAngle.getValue(), mProp.mMirror.getValue(), mProp.mSide.getValue() );
  QTransform t = impl.getMatrix();
  for( SdPartImpPin &pin : mPins )
    pin.mPosition = t.map( pin.mPin->getPinOrigin() );
  mOverRect.set( t.mapRect( mPart->getOverRect() ) );
  mIdentPos = t.map( mIdentOrigin );
  }
