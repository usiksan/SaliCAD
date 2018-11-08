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
#include "SdGraphValue.h"
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
#include "SdObjectFactory.h"

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
  //Draw pin pad
  plate->drawPad( dc, mPin->getPinOrigin(), mPin->getPinType(), mStratum.stratum() & stratum );
  //Draw pin connection and also pin name and number
  drawWithoutPad( dc );
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
//  plate->drawPad( dc, mPosition, mPin->getPinType(), mStratum.stratum() & stratum );
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




void SdPartImpPin::accumBarriers(SdPItemPlate *plate, SdBarrierList &dest, int stratum, SdRuleId ruleId, int clearance, int halfWidth, QTransform t ) const
  {

  //Compare on stratum
  if( mStratum & stratum ) {
    if( ruleId >= ruleLast )
      clearance = 0;
    else
      clearance = qMax( clearance, plate->ruleForNet( getNetName(), ruleId ) );
    SdBarrier bar;
    bar.mNetName = getNetName();
    bar.mPolygon = t.map( plate->getPadPolygon( mPin->getPinOrigin(), mPin->getPinType(), clearance + halfWidth) );
    dest.append( bar );
    }
  }




void SdPartImpPin::accumWindows(SdPItemPlate *plate, SdPolyWindowList &dest, int stratum, int gap, const QString netName, const QTransform &t) const
  {
  //Compare on stratum
  if( (mStratum & stratum) && netName != getNetName() )
    //Stratum matched and net name other
    plate->appendPadWindow( dest, mPin->getPinOrigin(), mPin->getPinType(), gap, t );
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
  mParamTable(param)           //Component params
  {
  //Implement properties
  mProp = *prp;
  if( part ) {
    QTransform t( matrix() );
    mOverRect.set( t.mapRect(part->getOverRect(dctAll & (~dctIdent))) );//Over rect
    mIdent.mProp = part->identGet()->getPropText();   //Part identificator text properties
    mIdent.mOrigin = part->identGet()->getOrigin();   //Part identificator position in part context
    mValue.mProp = part->valueGet()->getPropText();   //Part value text properties
    mValue.mOrigin = part->valueGet()->getOrigin();   //Part value position in part context
    }
  }

QTransform SdGraphPartImp::matrix() const
  {
  SdConverterImplement imp( mOrigin, mPart->getOrigin(), mProp.mAngle.getValue(), mProp.mSide.isBottom() );
  return imp.getMatrix();
  }










//Get full visual ident of part aka D4 or R45
QString SdGraphPartImp::ident() const
  {
  return getIdentPrefix() + QString::number(mLogNumber);
  }






//Set ident text properties and position
void SdGraphPartImp::identSet(const SdPropText &prp, SdPoint pos, SdUndo *undo)
  {
  if( undo )
    undo->propTextAndText( &mIdent.mProp, &mIdent.mOrigin, nullptr, nullptr );
  mIdent.mProp = prp;
  mIdent.mOrigin = pos;
  }





//Get full visual value of part aka smt32f417vgt
QString SdGraphPartImp::value() const
  {
  return paramGet(stdParamValue);
  }





//Set value text properties and position
void SdGraphPartImp::valueSet(const SdPropText &prp, SdPoint pos, SdUndo *undo)
  {
  if( undo )
    undo->propTextAndText( &mValue.mProp, &mValue.mOrigin, nullptr, nullptr );
  mValue.mProp = prp;
  mValue.mOrigin = pos;
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





QString SdGraphPartImp::getIdentPrefix() const
  {
  return paramGet( stdParamPrefix );
  }




bool SdGraphPartImp::compareRenumeration(const SdGraphPartImp *imp) const
  {
  SdPoint p1,p2;
  int sheet1,sheet2;
  if( getLowerPosAndSheet( p1, sheet1 ) && imp->getLowerPosAndSheet(p2, sheet2) ) {
    if( sheet2 > sheet1 ) return true;
    if( sheet2 == sheet1 ) {
      if( p2.x() > p1.x() ) return true;
      if( p1.x() == p2.x() ) {
        if( p2.y() < p1.y() ) return true;
        }
      }
    }
  return false;
  }





//Lower sheet
bool SdGraphPartImp::getLowerPosAndSheet(SdPoint &dest, int &sheet) const
  {
  int i = 0;
  //Find presented section
  while( i < mSections.count() ) {
    if( mSections.at(i).mSymImp ) {
      //Section present
      mSections.at(i).mSymImp->getRenumSect( dest, sheet );
      break;
      }
    i++;
    }

  if( i >= mSections.count() )
    //No presented section
    return false;

  //With next we select from all sections the section with lowerst sheet number
  // and more left and top
  i++;
  while( i < mSections.count() ) {
    if( mSections.at(i).mSymImp ) {
      //Section present
      SdPoint p;
      int s;
      mSections.at(i).mSymImp->getRenumSect( p, s );
      if( sheet < s ) {
        //This section in early sheet
        dest  = p;
        sheet = s;
        }
      else if( sheet == s ) {
        if( p.x() < dest.x() )
          //This section is lefter
          dest  = p;
        else if( p.x() == dest.x() ) {
          if( p.y() > dest.y() )
            //This section is upper
            dest  = p;
          }
        }
      }
    }

  return true;
  }




//Set index
void SdGraphPartImp::setIdentIndex(int index)
  {
  //Assign log number for part implement
  mLogNumber = index;
  //Assign log numbers for each connected section
  int log = mSections.count() > 1 ? 1 : 0;
  for( int i = 0; i < mSections.count(); i++ )
    if( mSections.at(i).mSymImp )
      mSections.at(i).mSymImp->identInfoSet( mLogNumber, log++ );
  }






//Setup full param table
void SdGraphPartImp::paramTableSet(const SdStringMap map, SdUndo *undo, SdGraphSymImp *symImp)
  {
  //If there undo then save previous param table
  if( undo != nullptr )
    undo->stringMap( &mParamTable );

  //Setup new table
  mParamTable = map;

  //Replace for all sections except calling section, because it already changed
  for( int i = 0; i < mSections.count(); i++ )
    if( mSections.at(i).mSymImp && mSections.at(i).mSymImp != symImp )
      mSections.at(i).mSymImp->paramTableSet( map, undo, this );
  }








QString SdGraphPartImp::getBomItemLine() const
  {
  QString bom = paramGet( QStringLiteral("bom") );
  if( bom.isEmpty() )
    return bom;

  //For each param key test if it present in bom line
  for( auto iter = mParamTable.cbegin(); iter != mParamTable.cend(); iter++ ) {
    QString field = QString("<%1>").arg( iter.key() );
    if( bom.contains( field ) )
      bom.replace( field, iter.value() );
    }

  return bom;
  }





//Draw part without pads
void SdGraphPartImp::drawWithoutPads(SdContext *cdx)
  {
  //Convertor for symbol implementation
  SdConverterImplement imp( mOrigin, mPart->getOrigin(), mProp.mAngle.getValue(), mProp.mSide.isBottom() );
  cdx->setConverter( &imp );

  SdRect ov;
  cdx->text( mIdent.mOrigin, ov, ident(), mIdent.mProp );
  cdx->text( mValue.mOrigin, ov, value(), mValue.mProp );

  //Draw symbol except ident and pins
  mPart->forEach( dctAll & ~(dctPartPin | dctIdent | dctValue), [cdx] (SdObject *obj) -> bool {
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
  //Convertor for symbol implementation
  SdConverterImplement imp( mOrigin, mPart->getOrigin(), mProp.mAngle.getValue(), mProp.mSide.isBottom() );
  cdx->setConverter( &imp );
  //Draw pins
  for( const SdPartImpPin &pin : mPins )
    pin.drawPad( cdx, getPlate(), stratum, highlightNetName );
  }





//Draw rat net
void SdGraphPartImp::drawRatNet(SdContext *cdx, SdPlateNetList &netList)
  {
  for( const SdPartImpPin &pin : mPins )
    if( pin.isConnected() ) {
      SdPoint p1 = pin.mPosition;
      SdPoint p2 = netList.nearestPoint( pin.getNetName(), p1 );
      if( p1 != p2 )
        cdx->line( p1, p2 );
      }
  }




//Check if there free section slot
bool SdGraphPartImp::isSectionFree(int *section, SdPItemPart *part, SdPItemComponent *comp, const SdStringMap &param, SdPItemSymbol *sym)
  {
  if( mPart != part || mComponent != comp || mParamTable != param )
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
  Q_UNUSED(undo)
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
    if( s.mSymbol ) s.mSymbol->autoDelete( undo );
    }
  //Autodelete all referenced objects
  if( mComponent ) mComponent->autoDelete( undo );
  if( mPart ) mPart->autoDelete( undo );
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
  mIdent       = imp->mIdent;       //Part identificator
  mValue       = imp->mValue;       //Part value

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
  mIdent.write( QStringLiteral("Ident"), obj );
  mValue.write( QStringLiteral("Value"), obj );
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
  sdStringMapWrite( QStringLiteral("Param"), mParamTable, obj );
  }





void SdGraphPartImp::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraphTraced::readObject( map, obj );
  mLogNumber = obj.value( QStringLiteral("LogNum") ).toInt();
  mOrigin.read( QStringLiteral("Org"), obj );
  mProp.read( obj );
  mOverRect.read( QStringLiteral("Over"), obj );
  mIdent.read( QStringLiteral("Ident"), obj );
  mValue.read( QStringLiteral("Value"), obj );
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
  sdStringMapRead( QStringLiteral("Param"), mParamTable, obj );
  }




void SdGraphPartImp::saveState(SdUndo *undo)
  {
  undo->partImp( &mOrigin, &mProp, &mLogNumber, &mOverRect );
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
  if( !mProp.match( prop.mPartImpProp ) ) {
    mProp = prop.mPartImpProp;
    updatePinsPositions();
    }
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
  mIdent.mProp.mLayer.setLayerUsage();
  mValue.mProp.mLayer.setLayerUsage();
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
  //Convertor for part implementation
  SdConverterImplement imp( mOrigin, mPart->getOrigin(), mProp.mAngle.getValue(), mProp.mSide.isBottom() );
  dc->setConverter( &imp );

  //Draw ident in plate context
  SdRect ov;
  if( stratum == 0 || stratum == stmThrough ) {
    dc->text( mIdent.mOrigin, ov, ident(), mIdent.mProp );
    dc->text( mValue.mOrigin, ov, value(), mValue.mProp );
    }

  //Draw part except ident and pins
  if( stratum == 0 || stratum == stmThrough ) {
    mPart->forEach( dctAll & ~(dctPartPin | dctIdent | dctValue), [dc] (SdObject *obj) -> bool {
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

  SdConverterImplement imp( mOrigin, mPart->getOrigin(), mProp.mAngle.getValue(), mProp.mSide.isBottom() );
  QTransform t( imp.getMatrix() );

  for( const SdPartImpPin &pin : mPins )
    pin.accumBarriers( plate, dest, stratum, toWhich, clearance, halfWidth, t );
  }






void SdGraphPartImp::accumWindows(SdPolyWindowList &dest, int stratum, int gap, const QString netName) const
  {
  //Accum windows for all pin pads

  //Plate for pad association
  SdPItemPlate *plate = getPlate();

  //Component position and orientation converter
  SdConverterImplement imp( mOrigin, mPart->getOrigin(), mProp.mAngle.getValue(), mProp.mSide.isBottom() );
  QTransform t( imp.getMatrix() );

  //For each pin accum windows
  for( const SdPartImpPin &pin : mPins )
    pin.accumWindows( plate, dest, stratum, gap, netName, t );
  }














int SdGraphPartImp::behindCursor(SdPoint p)
  {
  if( mOverRect.isPointInside(p) )
    return getSelector() ? SEL_ELEM : UNSEL_ELEM;
  return 0;
  }




bool SdGraphPartImp::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  if( behindCursor( p ) ) {
    if( extInfo ) {
      info = getBomItemLine();
      if( !info.isEmpty() )
        return true;
      }
    info = ident();
    if( mComponent )
      info.append( QString("  ") ).append( mComponent->getTitle() );
    return true;
    }
  return false;
  }




//Find snap point on object
void SdGraphPartImp::snapPoint(SdSnapInfo *snap)
  {
  //We snap to pins
  if( snap->match( snapNearestPin | snapNearestNetPin ) ) {
    for( SdPartImpPin &pin : mPins )
      if( pin.isConnected() ) {
        //Perform snap only on connected pins
        if( (snap->match( snapNearestPin ) || snap->mNetName == pin.getNetName()) && snap->mStratum.match(pin.mStratum) )
          snap->test( this, pin.mPosition, snapNearestPin | snapNearestNetPin );
        }
    }
  }






void SdGraphPartImp::updatePinsPositions()
  {
  //Converter for calculate and get conversion matrix, which convert part origin referenced coords to plate referenced coords
  SdConverterImplement impl( mOrigin, mPart->getOrigin(), mProp.mAngle.getValue(), mProp.mSide.isBottom() );
  //Conversion matrix
  QTransform t = impl.getMatrix();
  //For each pin perform conversion
  for( SdPartImpPin &pin : mPins ) {
    //Convert pin position
    pin.mPosition = t.map( pin.mPin->getPinOrigin() );
    //qDebug() << "pin" << pin.mPosition;
    //Convert stratum stack for pin
    pin.mStratum  = pin.mPin->getPinStratum( mProp.mSide.isBottom() );
    }
  //Calculate new over rect
  mOverRect.set( t.mapRect( mPart->getOverRect(dctAll & (~dctIdent)) ) );
  }






bool SdGraphPartImp::isPointOnNet(SdPoint p, SdStratum stratum, QString *netName, int *destStratum)
  {
  //Run on each pin, test stratum and pos. If match then return true and assign wireName
  for( SdPartImpPin &pin : mPins ) {
    if( pin.isConnected() && pin.mPosition == p && pin.mStratum.match( stratum ) ) {
      if( *netName == pin.getNetName() )
        *destStratum |= pin.mStratum.getValue();
      else {
        *destStratum = pin.mStratum.getValue();
        *netName = pin.getNetName();
        }
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




bool SdGraphPartImp::upgradeProjectItem(SdUndo *undo, QWidget *parent)
  {
  if( SdObjectFactory::isThereNewer(mComponent) || SdObjectFactory::isThereNewer(mPart) ) {
    //There newer objects. Upgrade.
    if( mSections.count() )
      return false;

    //Prepare newer objects
    SdPItemComponent *comp = sdObjectOnly<SdPItemComponent>( mComponent ? SdObjectFactory::extractObject( mComponent->getUid(), false, parent ) : nullptr );
    SdPItemPart      *part = sdObjectOnly<SdPItemPart>( comp ? comp->extractPartFromFactory( false, parent ) : nullptr );

    //Test if all newer objects prepared
    if( comp && part ) {
      detach(undo);

      //Because "detach" perhaps without deleting symImp, then referenced object not autodeleted
      //Perform autodeling it
      SdPItemComponent *dcomp = mComponent;
      SdPItemPart      *dpart = mPart;
      //Setup newer objects
      mComponent = comp;
      mPart      = part;

      //Autodelete all referenced objects
      if( dcomp ) dcomp->autoDelete( undo );
      if( dpart ) dpart->autoDelete( undo );

      attach(undo);

      delete comp;
      delete part;
      return true;
      }
    if( comp ) delete comp;
    if( part ) delete part;
    return false;
    }
  return true;
  }




