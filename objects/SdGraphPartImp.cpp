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
  mCom(false),
  mPadStack(nullptr)  //Pad stack
  {
  }




void SdPartImpPin::operator =(const SdPartImpPin &pin)
  {
  mPin       = pin.mPin;
  mPinNumber = pin.mPinNumber; //Part pin number
  mPinName   = pin.mPinName;   //Part pin name
  mPosition  = pin.mPosition;  //Pin position in plate context
  mNetName   = pin.mNetName;
  mCom       = pin.mCom;
  mPadStack  = pin.mPadStack;  //Pad stack
  mStratum   = pin.mStratum;
  }






void SdPartImpPin::draw(SdContext *dc)
  {
  mPin->drawImp( dc, mPinName, mCom );
  }



QJsonObject SdPartImpPin::toJson() const
  {
  QJsonObject obj;
  SdObject::writePtr( mPin, QStringLiteral("Pin"), obj );          //Original pin
  obj.insert( QStringLiteral("PinNum"), mPinNumber );              //Part pin number
  obj.insert( QStringLiteral("PinNam"), mPinName );                //Part pin name
  obj.insert( QStringLiteral("Net"), mNetName );                   //Name of net pin conneted to
  mPosition.write( QStringLiteral("Pos"), obj );                   //Pin position in plate context
  obj.insert( QStringLiteral("Com"), mCom );                       //Pin to wire flag connection
  SdObject::writePtr( mPadStack, QStringLiteral("Pad"), obj );     //Pad stack
  mStratum.write( obj );
  return obj;
  }




void SdPartImpPin::fromJson(SdObjectMap *map, const QJsonObject obj)
  {
  mPin = dynamic_cast<SdGraphPartPin*>( SdObject::readPtr( QStringLiteral("Pin"), map, obj )  );
  mPinNumber = obj.value( QStringLiteral("PinNum") ).toString();
  mPinName = obj.value( QStringLiteral("PinNam") ).toString();            //Part pin name
  mNetName = obj.value( QStringLiteral("Net") ).toString();               //Name of net pin conneted to
  mPosition.read( QStringLiteral("Pos"), obj );                           //Pin position in plate context
  mCom = obj.value( QStringLiteral("Com") ).toBool();                     //Pin to wire flag connection
  mPadStack = dynamic_cast<SdPItemPart*>( SdObject::readPtr( QStringLiteral("Pad"), map, obj )  );
  mStratum.read( obj );
  }





//====================================================================================
//Section in part implementation
QJsonObject SdPartImpSection::toJson() const
  {
  QJsonObject obj;
  SdObject::writePtr( mSymbol, QStringLiteral("Sym"), obj );
  SdObject::writePtr( mSymImp, QStringLiteral("Imp"), obj );
  return obj;
  }


void SdPartImpSection::fromJson(SdObjectMap *map, const QJsonObject obj)
  {
  mSymbol = dynamic_cast<SdPItemSymbol*>( SdObject::readPtr( QStringLiteral("Sym"), map, obj )  );
  mSymImp = dynamic_cast<SdGraphSymImp*>( SdObject::readPtr( QStringLiteral("Imp"), map, obj )  );
  }



//====================================================================================
//Part implementation
SdGraphPartImp::SdGraphPartImp() :
  SdGraphTraced(),
  mPart(nullptr),
  mComponent(nullptr)
  {

  }

SdGraphPartImp::SdGraphPartImp(SdPoint org, SdPropPartImp *prp, SdPItemPart *part, SdPItemSymbol *comp) :
  SdGraphTraced(),
  mLogNumber(0),   //Logical part number (from 1)
  mOrigin(org),    //Position of Implement
  mPart(part),     //Part for this implementation
  mComponent(comp)      //Component
  {
  //Implement properties
  mProp = *prp;
  if( part ) {
    SdConverterImplement imp( mOrigin, part->getOrigin(), mProp.mAngle.getValue(), mProp.mMirror.getValue(), mProp.mSide.getValue() );
    QTransform t( imp.getMatrix() );
    mOverRect.set( t.mapRect(part->getOverRect()) );//Over rect
    mPrefix = part->getIdent()->getText();          //Part identificator prefix
    mIdentProp = part->getIdent()->getPropText();   //Part identificator text properties
    mIdentOrigin = part->getIdent()->getOrigin();   //Part identificator position in symbol context
    mIdentPos = t.map( mIdentOrigin );    //Part identificator position in sheet context
    mIdentRect.set( t.mapRect( part->getIdent()->getOverRect() )  );   //Part identificator over rect
    }
  }




void SdGraphPartImp::pinConnectionSet(int pinIndex, const QString netName, bool com)
  {
  //If no pinIndex then doing nothink
  if( pinIndex < 0 ) return;
  Q_ASSERT( pinIndex >= 0 && pinIndex < mPins.count() );
  mPins[pinIndex].mNetName = netName;
  mPins[pinIndex].mCom = com;
  setDirtyRatNet();
  }






//Get full visual ident of part aka D4 or R45
QString SdGraphPartImp::getIdent() const
  {
  return mPrefix + QString::number(mLogNumber);
  }



//Get wire name pin with pinIndex connected to
QString SdGraphPartImp::pinNetName(int pinIndex) const
  {
  if( pinIndex < 0 ) return QString();
  Q_ASSERT( pinIndex < mPins.count() );
  if( mPins[pinIndex].mCom )
    return mPins[pinIndex].mNetName;
  return QString();
  }




//Pin position
SdPoint SdGraphPartImp::pinPosition(int pinIndex) const
  {
  if( pinIndex < 0 ) return SdPoint();
  Q_ASSERT( pinIndex < mPins.count() );
  return mPins[pinIndex].mPin->getPinOrigin();
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




//Set pin name for pin index
void SdGraphPartImp::setPinName(int pinIndex, const QString pinName)
  {
  if( pinIndex < 0 ) return;
  Q_ASSERT( pinIndex < mPins.count() );
  mPins[pinIndex].mPinName = pinName;
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

  updatePinsPositions();
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
  SdGraphTraced::writeObject( obj );
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
  //Write sections
  QJsonArray sections;
  for( const SdPartImpSection &sec : mSections )
    sections.append( sec.toJson() );
  obj.insert( QStringLiteral("Sections"), sections );
  //Write pins
  QJsonArray pins;
  for( const SdPartImpPin &pin : mPins )
    pins.append( pin.toJson() );
  obj.insert( QStringLiteral("Pins"), pins );
  }





void SdGraphPartImp::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraphTraced::readObject( map, obj );
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
  //Read sections
  QJsonArray sections = obj.value( QStringLiteral("Sections") ).toArray();
  for( QJsonValue s : sections ) {
    SdPartImpSection sec;
    sec.fromJson( map, s.toObject() );
    mSections.append( sec );
    }

  //Pin information table
  QJsonArray pins = obj.value( QStringLiteral("Pins") ).toArray();
  for( QJsonValue vpin : pins ) {
    SdPartImpPin pin;
    pin.fromJson( map, vpin.toObject() );
    mPins.append( pin );
    }
  //Parameters
  //sdParamRead( QStringLiteral("Param"), mParam, obj );
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




void SdGraphPartImp::setProp(SdPropSelected &prop)
  {
  mProp = prop.mPartImpProp;
  }




void SdGraphPartImp::getProp(SdPropSelected &prop)
  {
  prop.mPartImpProp.append( mProp );
  prop.mFilledPropMask |= spsPartImpProp;
  }




void SdGraphPartImp::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( !getSelector() && mOverRect.isPointInside(p) )
    selector->insert( this );
  }




void SdGraphPartImp::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( !getSelector() && r.isAccross( mOverRect ) )
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
  SdConverterImplement imp( mOrigin, mPart->getOrigin(), mProp.mAngle.getValue(), mProp.mMirror.getValue(), mProp.mSide.getValue() );
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
  //TODO D024 getInfo partImp
  return false;
  }




bool SdGraphPartImp::snapPoint(SdSnapInfo *snap)
  {
  //TODO D010 snap partImp
  return false;
  }




void SdGraphPartImp::updatePinsPositions()
  {
  SdConverterImplement impl( mOrigin, mPart->getOrigin(), mProp.mAngle.getValue(), mProp.mMirror.getValue(), mProp.mSide.getValue() );
  QTransform t = impl.getMatrix();
  for( SdPartImpPin &pin : mPins ) {
    pin.mPosition = t.map( pin.mPin->getPinOrigin() );
    pin.mStratum  = pin.mPin->getPinStratum( mProp.mSide.getValue() );
    }
  mOverRect.set( t.mapRect( mPart->getOverRect() ) );
  mIdentPos = t.map( mIdentOrigin );
  }



bool SdGraphPartImp::isPointOnNet(SdPoint p, SdStratum stratum, QString &netName)
  {
  //Run on each pin, test stratum and pos. If match then return true and assign wireName
  for( SdPartImpPin &pin : mPins ) {
    if( pin.mCom && pin.mPosition == p && pin.mStratum.match( stratum ) ) {
      netName = pin.mNetName;
      return true;
      }
    }
  return false;
  }






void SdGraphPartImp::accumNetPoints(SdPlateNetList &netList)
  {
  for( SdPartImpPin &pin : mPins )
    if( pin.mCom )
      netList.addNetPoint( pin.mNetName, pin.mStratum, pin.mPosition );
  }




bool SdGraphPartImp::isUsed(SdObject *obj) const
  {
  return obj == mPart || obj == mComponent;
  }

