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
#include "SdPItemComponent.h"
#include "SdProject.h"
#include "SdContext.h"
#include "SdSelector.h"
#include "SdEnvir.h"
#include "SdPlateNetList.h"

#include <QDebug>


//====================================================================================
//Pin for part implementation
SdPartImpPin::SdPartImpPin() :
  mPin(nullptr),       //Original pin
  mSection(nullptr)    //Schematic section where pin is located
  {
  }




void SdPartImpPin::operator =(const SdPartImpPin &pin)
  {
  mPin       = pin.mPin;
  mSection   = pin.mSection;
  mPinName   = pin.mPinName;   //Part pin name
  mPosition  = pin.mPosition;  //Pin position in plate context
  }






void SdPartImpPin::draw(SdContext *dc, SdPItemPlate *plate, int stratum ) const
  {
  drawWithoutPad( dc );
  //Draw pin pad
  plate->drawPad( dc, mPin->getPinOrigin(), mPin->getPinType(), mStratum.stratum() & stratum );
  }



void SdPartImpPin::drawWithoutPad(SdContext *dc) const
  {
  //Draw standard pin (pin connection point, pin number and pin name)
  mPin->drawImp( dc, mPinName, isConnected() );
  }



void SdPartImpPin::drawPad(SdContext *dc, SdPItemPlate *plate, int stratum, const QString highliteNet ) const
  {
  //Draw pin pad
  if( !highliteNet.isEmpty() && getNetName() == highliteNet )
    dc->setOverColor( sdEnvir->getSysColor(scTraseNet) );
  plate->drawPad( dc, mPin->getPinOrigin(), mPin->getPinType(), mStratum.stratum() & stratum );
  dc->resetOverColor();
  }




bool SdPartImpPin::isConnected() const
  {
  return mSection != nullptr && mSection->isPinConnected(mPinName);
  }




QString SdPartImpPin::getNetName() const
  {
  if( mSection != nullptr )
    return mSection->pinNetName( mPinName );
  return QString();
  }





void SdPartImpPin::accumUsedPin(SdPadMap &map) const
  {
  if( !map.contains(mPin->getPinType()) )
    //Append new pin-to-pad association with default pad
    map.insert( mPin->getPinType(), sdEnvir->getPad(mPin->getPinType()) );
  }




void SdPartImpPin::accumBarriers(SdPItemPlate *plate, SdBarrierList &dest, int stratum, SdRuleId ruleId, int clearance, int halfWidth ) const
  {

  //Compare on stratum
  if( mStratum & stratum ) {
    if( ruleId >= ruleLast )
      clearance = 0;
    else
      clearance = qMax( clearance, plate->ruleForNet( stratum, getNetName(), ruleId ) );
    SdBarrier bar;
    bar.mNetName = getNetName();
    bar.mPolygon = plate->getPadPolygon( mPosition, mPin->getPinType(), clearance + halfWidth);
    dest.append( bar );
    }
  }




QJsonObject SdPartImpPin::toJson( const QString pinNumber ) const
  {
  QJsonObject obj;
  SdObject::writePtr( mPin, QStringLiteral("Pin"), obj );          //Original pin
  SdObject::writePtr( mSection, QStringLiteral("Section"), obj );  //Section
  obj.insert( QStringLiteral("PinNum"), pinNumber );               //Part pin number
  obj.insert( QStringLiteral("PinNam"), mPinName );                //Part pin name
  mPosition.write( QStringLiteral("Pos"), obj );                   //Pin position in plate context
  mStratum.writeStratum( obj );
  return obj;
  }




QString SdPartImpPin::fromJson(SdObjectMap *map, const QJsonObject obj)
  {
  mPin = dynamic_cast<SdGraphPartPin*>( SdObject::readPtr( QStringLiteral("Pin"), map, obj )  );
  mSection = dynamic_cast<SdGraphSymImp*>( SdObject::readPtr( QStringLiteral("Section"), map, obj )  );
  mPinName = obj.value( QStringLiteral("PinNam") ).toString();            //Part pin name
  mPosition.read( QStringLiteral("Pos"), obj );                           //Pin position in plate context
  mStratum.readStratum( obj );
  return obj.value( QStringLiteral("PinNum") ).toString();
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

SdGraphPartImp::SdGraphPartImp(SdPoint org, SdPropPartImp *prp, SdPItemPart *part, SdPItemComponent *comp, const SdStringMap &param) :
  SdGraphTraced(),
  mLogNumber(0),          //Logical part number (from 1)
  mOrigin(org),           //Position of Implement
  mPart(part),            //Part for this implementation
  mComponent(comp),       //Component
  mParam(param)           //Component params
  {
  //Implement properties
  mProp = *prp;
  if( part ) {
    SdConverterImplement imp( mOrigin, part->getOrigin(), mProp.mAngle.getValue(), mProp.mSide.isBottom() );
    QTransform t( imp.getMatrix() );
    mOverRect.set( t.mapRect(part->getOverRect()) );//Over rect
    mPrefix = part->getIdent()->getText();          //Part identificator prefix
    mIdentProp = part->getIdent()->getPropText();   //Part identificator text properties
    mIdentOrigin = part->getIdent()->getOrigin();   //Part identificator position in symbol context
    mIdentPos = t.map( mIdentOrigin );    //Part identificator position in sheet context
    mIdentRect.set( t.mapRect( part->getIdent()->getOverRect() )  );   //Part identificator over rect
    }
  }










//Get full visual ident of part aka D4 or R45
QString SdGraphPartImp::getIdent() const
  {
  return mPrefix + QString::number(mLogNumber);
  }









void SdGraphPartImp::setLinkSection(int section, SdGraphSymImp *symImp)
  {
  if( section >= 0 && section < mSections.count() )
    mSections[section].mSymImp = symImp;
  }



//Check if all section removed, then autodeleted
void SdGraphPartImp::autoDelete(SdUndo *undo)
  {
  for( const SdPartImpSection &sect : mSections )
    if( sect.mSymImp != nullptr ) return;
  deleteObject( undo );
  }





//Save to undo state of all pins
void SdGraphPartImp::savePins(SdUndo *undo)
  {
  //Save state of all pins
  undo->partImpPins( &mPins );
  }




//Accum used pins
void SdGraphPartImp::accumUsedPins(SdPadMap &map) const
  {
  for( const SdPartImpPin &pin : mPins )
    pin.accumUsedPin( map );
  }




void SdGraphPartImp::setParam(const QString key, const QString val, SdUndo *undo)
  {
  undo->stringMapItem( &mParam, key );
  mParam.insert( key, val );
  }





//Draw part without pads
void SdGraphPartImp::drawWithoutPads(SdContext *cdx)
  {
  cdx->text( mIdentPos, mIdentRect, getIdent(), mIdentProp );
  //Convertor for symbol implementation
  SdConverterImplement imp( mOrigin, mPart->getOrigin(), mProp.mAngle.getValue(), mProp.mSide.isBottom() );
  cdx->setConverter( &imp );

  //Draw symbol except ident and pins
  mPart->forEach( dctAll & ~(dctPartPin | dctIdent), [cdx] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph )
      graph->draw( cdx );
    return true;
    });

  //Draw pins
  for( const SdPartImpPin &pin : mPins )
    pin.drawWithoutPad( cdx );
  }




//Draw pads only
void SdGraphPartImp::drawPads(SdContext *cdx, SdStratum stratum, const QString highlightNetName)
  {
  //Draw pins
  for( const SdPartImpPin &pin : mPins )
    pin.drawPad( cdx, getPlate(), stratum, highlightNetName );
  }




//Check if there free section slot
bool SdGraphPartImp::isSectionFree(int *section, SdPItemPart *part, SdPItemComponent *comp, const SdStringMap &param, SdPItemSymbol *sym)
  {
  if( mPart != part || mComponent != comp || mParam != param )
    return false;
  for( int index = 0; index < mSections.count(); index++ )
    if( mSections[index].isFree(sym) ) {
      *section = index;
      return true;
      }
  return false;
  }





//Pin link-unlink
bool SdGraphPartImp::partPinLink(const QString pinNumber, SdGraphSymImp *imp, const QString pinName, bool link)
  {
  if( !mPins.contains(pinNumber) )
    return false;
  if( link ) {
    //Link pins
    if( mPins[pinNumber].mSection != nullptr )
      return false;
    mPins[pinNumber].mSection = imp;
    mPins[pinNumber].mPinName = pinName;
    return true;
    }
  //Unlink pins
  if( mPins[pinNumber].mSection != imp )
    return false;
  mPins[pinNumber].mSection = nullptr;
  mPins[pinNumber].mPinName.clear();
  return true;
  }








void SdGraphPartImp::attach(SdUndo *undo)
  {
  SdPItemPlate *plate = getPlate();
  Q_ASSERT( plate != nullptr );
  SdProject *prj = plate->getProject();
  Q_ASSERT( prj != nullptr );
  //Realloc objects for this project
  mComponent = dynamic_cast<SdPItemComponent*>( prj->getFixedProjectItem(mComponent) );  //Object contains section information, pin assotiation info. May be same as mSymbol.
  mPart = dynamic_cast<SdPItemPart*>( prj->getFixedProjectItem(mPart) );

  //Fill pin table
  mPins.clear();
  if( mPart ) {
    mPart->forEach( dctPartPin, [this] (SdObject *obj) -> bool {
      SdGraphPartPin *pin = dynamic_cast<SdGraphPartPin*>(obj);
      Q_ASSERT( pin != nullptr );

      //Duplicate pin numbers not allowed
      if( mPins.contains(pin->getPinNumber()) )
        return true;

      //Create implement pin
      SdPartImpPin impPin;
      impPin.mPin       = pin;

      //Add pin to table
      mPins.insert( pin->getPinNumber(), impPin );
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
      s.mSymbol = dynamic_cast<SdPItemSymbol*>( prj->getFixedProjectItem(sym) );
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
      s.mSymImp->unLinkPart( undo );
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
  for( SdPartImpPinTable::const_iterator i = mPins.constBegin(); i != mPins.constEnd(); i++ )
    pins.append( i.value().toJson( i.key() ) );
  obj.insert( QStringLiteral("Pins"), pins );
  //Parameters
  sdStringMapWrite( QStringLiteral("Param"), mParam, obj );
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
  mComponent = dynamic_cast<SdPItemComponent*>( readPtr( QStringLiteral("Comp"), map, obj )  );
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
    QString pinNumber = pin.fromJson( map, vpin.toObject() );
    mPins.insert( pinNumber, pin );
    }
  //Parameters
  sdStringMapRead( QStringLiteral("Param"), mParam, obj );
  }




void SdGraphPartImp::saveState(SdUndo *undo)
  {
  undo->partImp( &mOrigin, &mProp, &mLogNumber, &mOverRect, &mPrefix, &mIdentProp, &mIdentOrigin, &mIdentPos, &mIdentRect );
  //Save state of all pins
  undo->partImpPins( &mPins );
  }





void SdGraphPartImp::move(SdPoint offset)
  {
  mOrigin.move( offset );
  updatePinsPositions();
  }




void SdGraphPartImp::rotate(SdPoint center, SdPropAngle angle)
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




void SdGraphPartImp::setLayerUsage()
  {
  mIdentProp.mLayer.setLayerUsage();
  }






bool SdGraphPartImp::isVisible()
  {
  return true;
  }




SdRect SdGraphPartImp::getOverRect() const
  {
  return mOverRect;
  }




void SdGraphPartImp::drawStratum(SdContext *dc, int stratum)
  {
  //Draw ident in plate context
  if( stratum == 0 || stratum == stmThrow )
    dc->text( mIdentPos, mIdentRect, getIdent(), mIdentProp );
  //Convertor for symbol implementation
  SdConverterImplement imp( mOrigin, mPart->getOrigin(), mProp.mAngle.getValue(), mProp.mSide.isBottom() );
  dc->setConverter( &imp );

  //Draw symbol except ident and pins
  if( stratum == 0 || stratum == stmThrow ) {
    mPart->forEach( dctAll & ~(dctPartPin | dctIdent), [dc] (SdObject *obj) -> bool {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph )
        graph->draw( dc );
      return true;
      });
    }

  //Draw pins
  for( const SdPartImpPin &pin : mPins )
    pin.draw( dc, getPlate(), stratum );
  }




//Accum barriers for all pin pad. It allow accum clear pad, pad with clearance only or pad with clearance and road width
void SdGraphPartImp::accumBarriers(SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk) const
  {
  //Accum barriers for all pin pad
  SdPItemPlate *plate = getPlate();
  //Addon clearance and halfWidth
  //Clearance needed to determine which clearance is max one and use
  //halfWidth needed to trace road as line with null width because absent intersection
  //allow trace road
  int clearance, halfWidth;
  if( toWhich == ruleRoadWidth )
    clearance = halfWidth = 0;
  else {
    if( toWhich == ruleRoadRoad ) toWhich = ruleRoadPad;
    clearance = blk.mRules[toWhich];
    halfWidth = blk.mRules[ruleRoadWidth] / 2;
    }
  for( const SdPartImpPin &pin : mPins )
    pin.accumBarriers( plate, dest, stratum, toWhich, clearance, halfWidth  );
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
  SdConverterImplement impl( mOrigin, mPart->getOrigin(), mProp.mAngle.getValue(), mProp.mSide.isBottom() );
  QTransform t = impl.getMatrix();
  for( SdPartImpPin &pin : mPins ) {
    pin.mPosition = t.map( pin.mPin->getPinOrigin() );
    qDebug() << "pin" << pin.mPosition;
    pin.mStratum  = pin.mPin->getPinStratum( mProp.mSide.isBottom() );
    }
  mOverRect.set( t.mapRect( mPart->getOverRect() ) );
  mIdentPos = t.map( mIdentOrigin );
  }



bool SdGraphPartImp::isPointOnNet(SdPoint p, SdStratum stratum, QString *netName, int *destStratum)
  {
  //Run on each pin, test stratum and pos. If match then return true and assign wireName
  for( SdPartImpPin &pin : mPins ) {
    if( pin.isConnected() && pin.mPosition == p && pin.mStratum.match( stratum ) ) {
      *destStratum = pin.mStratum.getValue();
      *netName = pin.getNetName();
      return true;
      }
    }
  return false;
  }



void SdGraphPartImp::accumNetSegments(SdPlateNetList &netList) const
  {
  for( const SdPartImpPin &pin : mPins )
    if( pin.isConnected() )
      netList.addNetSegment( pin.getNetName(), pin.mStratum, pin.mPosition, pin.mPosition );
  }







bool SdGraphPartImp::isUsed(SdObject *obj) const
  {
  return obj == mPart || obj == mComponent;
  }




void SdGraphPartImp::upgradeProjectItem(SdProjectItem *newItem, SdUndo *undo)
  {

  }




