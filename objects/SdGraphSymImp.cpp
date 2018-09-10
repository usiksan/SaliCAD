/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdGraphSymImp.h"
#include "SdGraphPartImp.h"
#include "SdPItemSymbol.h"
#include "SdGraphSymPin.h"
#include "SdGraphIdent.h"
#include "SdGraphArea.h"
#include "SdPItemPlate.h"
#include "SdPItemSheet.h"
#include "SdPItemPart.h"
#include "SdGraphNetWire.h"
#include "SdProject.h"
#include "SdSection.h"
#include "SdSelector.h"
#include "SdConverterImplement.h"
#include "SdContext.h"
#include "SdUndo.h"
#include "SdEnvir.h"

#include <QDebug>



//====================================================================================
//Symbol implementation
SdGraphSymImp::SdGraphSymImp() :
  mArea(nullptr),        //PCB where this symbol implement contains in
  mSectionIndex(0),      //Section index (from 0)
  mLogSection(0),        //Logical symbol section number (from 1)
  mLogNumber(0),         //Logical part number (from 1)
  mComponent(nullptr),   //Object contains section information, pin assotiation info. May be same as mSymbol.
  mSymbol(nullptr),      //Symbol contains graph information
  mPart(nullptr),
  mPartImp(nullptr)
  {

  }

SdGraphSymImp::SdGraphSymImp(SdPItemComponent *comp, SdPItemSymbol *sym, SdPItemPart *part, const SdStringMap &param, SdPoint pos, SdPropSymImp *prp ) :
  mArea(nullptr),        //PCB where this symbol implement contains in
  mSectionIndex(0),      //Section index (from 0)
  mLogSection(0),        //Logical symbol section number (from 1)
  mLogNumber(0),         //Logical part number (from 1)
  mOrigin(pos),          //Position of Implement
  mComponent(comp),      //Object contains section information, pin assotiation info. May be same as mSymbol.
  mSymbol(sym),          //Symbol contains graph information
  mPart(part),
  mPartImp(nullptr),
  mParamTable(param)
  {
  //QString           mName;        //Name of component
  mProp = *prp;        //Implement properties
  if( sym ) {
    SdConverterImplement imp( mOrigin, sym->getOrigin(), mProp.mAngle.getValue(), mProp.mMirror.getValue() );
    QTransform t( imp.getMatrix() );
    mOverRect.set( t.mapRect(sym->getOverRect(dctAll & (~dctIdent))) );//Over rect
    mIdentPrefix = sym->getIdent()->getText();          //Part identificator prefix
    mIdentProp = sym->getIdent()->getPropText();   //Part identificator text properties
    mIdentOrigin = sym->getIdent()->getOrigin();   //Part identificator position in symbol context
    mIdentPos = t.map( mIdentOrigin );    //Part identificator position in sheet context
    mIdentRect.set( t.mapRect( sym->getIdent()->getOverRect() )  );   //Part identificator over rect
    }
  }





//Retrive origin point and sheet number
QString SdGraphSymImp::getRenumSect(SdPoint &dest, int &sheetNumber) const
  {
  dest = mOverRect.getTopLeft();
  sheetNumber = getSheet()->getSheetIndex();
  return mIdentPrefix;
  }




//Pin connection status
bool SdGraphSymImp::isPinConnected(const QString pinName) const
  {
  if( mPins.contains(pinName) )
    return mPins.value( pinName ).isConnected();
  return false;
  }




//Pin net name
QString SdGraphSymImp::pinNetName(const QString pinName) const
  {
  if( mPins.contains(pinName) )
    return mPins.value( pinName ).mNetName;
  return QString();
  }





//Setup full param table
void SdGraphSymImp::paramTableSet(const SdStringMap map, SdUndo *undo, SdGraphPartImp *partImp)
  {
  //If there undo then save previous param table
  if( undo != nullptr )
    undo->stringMap( &mParamTable );

  //Setup new table
  mParamTable = map;

  //If called not from partImp and mPartImp present then replace table in mPartImp
  if( partImp == nullptr && mPartImp )
    mPartImp->paramTableSet( map, undo, this );
  }










void SdGraphSymImp::moveToPlate(SdPItemPlate *plate, SdUndo *undo)
  {
  //UnLink part in current plate
  unLinkPart( undo );

  //And link to part in another plate
  linkAutoPartInPlate( plate, undo );
  }




void SdGraphSymImp::setLinkSection(int section, SdGraphPartImp *partImp )
  {
  mPartImp = partImp;
  mSectionIndex = section;
  }




//Unconnect pin in point
void SdGraphSymImp::unconnectPinInPoint( SdPoint p, SdUndo *undo, const QString undoTitle )
  {
  for( SdSymImpPinTable::const_iterator i = mPins.constBegin(); i != mPins.constEnd(); i++ )
    if( i.value().mPosition == p ) {
      if( undo ) undo->begin( undoTitle, getSheet() );
      //Set new state of pin
      pinConnectionSet( i.key(), QString(), undo );
      return;
      }
  }




void SdGraphSymImp::unLinkPart(SdUndo *undo)
  {
  if( mPartImp != nullptr ) {
    //Save pin state
    undo->symImpPins( &mPins );
    //undo->partImpPins( )
    //UnLink pins if mPartImp present
    for( SdSymImpPinTable::iterator i = mPins.begin(); i != mPins.end(); i++ )
      mPartImp->partPinLink( i.value().mPinNumber, nullptr, QString(), undo );
    //Remove previous pins
    mPins.clear();
    //Save current link state
    undo->linkSection( mSectionIndex, this, mPartImp, mPartImp != nullptr );
    mPartImp->setLinkSection( mSectionIndex, nullptr );
    mPartImp->autoDelete( undo );
    setLinkSection( -1, nullptr );
    }
  }




//Link auto partImp. partImp and section are selected automatic
void SdGraphSymImp::linkAutoPart(SdUndo *undo)
  {
  //Get plate where component resides
  SdPItemPlate *plate = getSheet()->getPlate( mOrigin );
  Q_ASSERT( plate != nullptr );

  linkAutoPartInPlate( plate, undo );
  }









//Get BOM item line
QString SdGraphSymImp::getBomItemLine() const
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




//Get full visual ident of section aka D4.2
QString SdGraphSymImp::getIdent() const
  {
  if( mLogSection )
    return QString("%1%2.%3").arg( mIdentPrefix ).arg( mLogNumber ).arg( mLogSection );
  return QString("%1%2").arg( mIdentPrefix ).arg( mLogNumber );
  }




//Get separated ident information
QString SdGraphSymImp::getIdentInfo(int &logNumber, int &logSection)
  {
  logNumber = mLogNumber;
  logSection = mLogSection;
  return mIdentPrefix;
  }




//Set ident information
void SdGraphSymImp::setIdentInfo(const QString prefix, int logNumber, int logSection)
  {
  mIdentPrefix = prefix;
  mLogNumber = logNumber;
  mLogSection = logSection;
  }



//Move ident regarding symbol implement
void SdGraphSymImp::moveIdent(SdPoint offset)
  {
  //TODO D004 update origin regarding symbol
  mIdentOrigin = mIdentPos.unConvertImplement( mOrigin, offset, mProp.mAngle, mProp.mMirror.getValue() );
  mIdentPos.move( offset );
  }




//Get ident properties
void SdGraphSymImp::getIdentProp(SdProp &prop)
  {
  prop.mSymIdentProp.append( mIdentProp );
  }



//Set ident properties
void SdGraphSymImp::setIdentProp(const SdProp &prop)
  {
  mIdentProp = prop.mSymIdentProp;
  }



//Notification about wire segment position changed
void SdGraphSymImp::netWirePlace(SdGraphNetWire *wire, SdUndo *undo)
  {
  SdPoint a = wire->getA();
  SdPoint b = wire->getB();
  for( SdSymImpPinTable::const_iterator i = mPins.constBegin(); i != mPins.constEnd(); i++ )
    if( i.value().isCanConnect( a, b ) )
      //Set new state of pin
      pinConnectionSet( i.key(), wire->getNetName(), undo );
  }




//Notification about wire segment deletion
void SdGraphSymImp::netWireDelete(SdGraphNetWire *wire, SdUndo *undo)
  {
  SdPoint a = wire->getA();
  SdPoint b = wire->getB();
  QString name = wire->getNetName();
  for( SdSymImpPinTable::const_iterator i = mPins.constBegin(); i != mPins.constEnd(); i++ )
    if( i.value().isCanDisconnect( a, b, name ) )
      //Set new state of pin
      pinConnectionSet( i.key(), QString(), undo );
  }




//Accumulate segments connected to component
void SdGraphSymImp::accumLinked(SdPoint a, SdPoint b, const QString netName, SdSelector *sel)
  {
  if( getSelector() == nullptr ) {
    //Scan all pins and check if segment ab connected to any pin then select component
    for( SdSymImpPin &pin : mPins )
      if( pin.isConnected() && pin.mNetName == netName && pin.mPosition.isOnSegment(a,b) ) {
        sel->insert( this );
        return;
        }
    }
  }





//Pin status get
void SdGraphSymImp::pinStatusGet(const QString pinName, SdSymImpPin &pin) const
  {
  Q_ASSERT( mPins.contains(pinName) );
  pin = mPins.value(pinName);
  }




void SdGraphSymImp::pinStatusSet(const QString pinName, const SdSymImpPin &pin)
  {
  Q_ASSERT( mPins.contains(pinName) );
  mPins.insert( pinName, pin );
  }



void SdGraphSymImp::saveState(SdUndo *undo)
  {
  undo->symImp( &mOrigin, &mProp, &mLogSection, &mLogNumber, &mOverRect, &mIdentPrefix, &mIdentProp, &mIdentOrigin, &mIdentPos, &mIdentRect );
  //Save state of all pins
  undo->symImpPins( &mPins );
  }




void SdGraphSymImp::moveComplete(SdPoint grid, SdUndo *undo)
  {
  Q_UNUSED(grid);
  //Mooving is completed, check pin connection
  SdPItemSheet *sheet = getSheet();
  Q_ASSERT( sheet != nullptr );
  //Check current area
  if( mPartImp != nullptr ) {
    //Part is assigned, check current area
    if( sheet->getPlate( mOrigin ) != mPartImp->getPlate() )
      //Plate is changed. Unlink from part
      unLinkPart( undo );
    else {
      //Check for new pin connections
      updatePinsConnectionStatus( undo );
      return;
      }
    }
  //Assign part
  attach( undo );
  }




void SdGraphSymImp::prepareMove(SdUndo *undo)
  {
  Q_UNUSED(undo)
  //Prepare to move - accept linked wire segments
  SdSelector *sel = getSelector();
  SdPItemSheet *sheet = getSheet();
  if( sel && sheet ) {
    for( SdSymImpPin &pin : mPins )
      pin.prepareMove( sheet, sel, undo );
    }
  }



void SdGraphSymImp::move(SdPoint offset)
  {
  mOrigin.move( offset );
  updatePinsPositions();
  }




void SdGraphSymImp::rotate(SdPoint center, SdPropAngle angle)
  {
  mOrigin.rotate( center, angle );
  mProp.mAngle += angle;
  updatePinsPositions();
  }



void SdGraphSymImp::mirror(SdPoint a, SdPoint b)
  {
  mOrigin.mirror( a, b );
  updatePinsPositions();
  }



void SdGraphSymImp::setProp(SdPropSelected &prop)
  {
  if( !mProp.match( prop.mSymImpProp ) ) {
    //Unconnect all pins from wires
    //ucomAllPins();
    //setup props
    mProp = prop.mSymImpProp;
    //Correct pins positions
    updatePinsPositions();
    }
  }



void SdGraphSymImp::getProp(SdPropSelected &prop)
  {
  prop.mSymImpProp.append( mProp );
  prop.mFilledPropMask |= spsSymImpProp;
  }



void SdGraphSymImp::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( !getSelector() && mOverRect.isPointInside(p) )
    selector->insert( this );
  }




void SdGraphSymImp::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( !getSelector() && r.isAccross( mOverRect ) )
    selector->insert( this );
  }




bool SdGraphSymImp::isVisible()
  {
  return true;
  }



SdRect SdGraphSymImp::getOverRect() const
  {
  return mOverRect;
  }




void SdGraphSymImp::draw(SdContext *dc)
  {
  //Draw ident in sheet context
  dc->text( mIdentPos, mIdentRect, getIdent(), mIdentProp );
  //Convertor for symbol implementation
  SdConverterImplement imp( mOrigin, mSymbol->getOrigin(), mProp.mAngle.getValue(), mProp.mMirror.getValue() );
  dc->setConverter( &imp );

  //Draw symbol except ident and pins
  mSymbol->forEach( dctAll & ~(dctSymPin | dctIdent), [dc] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph )
      graph->draw( dc );
    return true;
    });

  //Draw pins
  for( SdSymImpPin &pin : mPins )
    pin.draw( dc );
  }




int SdGraphSymImp::behindCursor(SdPoint p)
  {
  if( mOverRect.isPointInside(p) )
    return getSelector() ? SEL_ELEM : UNSEL_ELEM;
  return 0;
  }





bool SdGraphSymImp::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  if( behindCursor( p ) ) {
    if( extInfo ) {
      QString bom;
      if( mPartImp )
        bom = mPartImp->getBomItemLine();
      else
        bom = getBomItemLine();
      if( !bom.isEmpty() ) {
        info = getIdent() + QString("  ") + bom;
        return true;
        }
      }
    info = getIdent();
    if( mComponent )
      info.append( QString("  ") ).append( mComponent->getTitle() );
    return true;
    }
  return false;
  }





bool SdGraphSymImp::snapPoint(SdSnapInfo *snap)
  {
  bool res = false;
  if( snap->match(snapNextPin) ) {
    //Must be checking with direction control
    for( SdSymImpPin &pin : mPins )
      if( !pin.isConnected() &&
          calcDirection90( snap->mSour, pin.mPosition ) == snap->mDir &&
          snap->test( pin.mPosition, snapNextPin ) ) res = true;
    }
  if( snap->match(snapNearestPin|snapNearestNet) ) {
    for( SdSymImpPin &pin : mPins )
      if( !pin.isConnected() && snap->test( pin.mPosition, snapNearestPin ) ) res = true;
    }
  return res;
  }




SdPItemSheet *SdGraphSymImp::getSheet() const
  {
  return dynamic_cast<SdPItemSheet*>( getParent() );
  }




void SdGraphSymImp::updatePinsPositions()
  {
  SdConverterImplement impl( mOrigin, mSymbol->getOrigin(), mProp.mAngle.getValue(), mProp.mMirror.getValue() );
  QTransform t = impl.getMatrix();
  for( SdSymImpPin &pin : mPins )
    pin.mPosition = t.map( pin.mPin->getPinOrigin() );
  mOverRect.set( t.mapRect( mSymbol->getOverRect(dctAll & (~dctIdent)) ) );
  mIdentPos = t.map( mIdentOrigin );
  //mIdentRect.set( )
  }








//Link auto partImp in given plate. partImp and section are selected automatic
void SdGraphSymImp::linkAutoPartInPlate(SdPItemPlate *plate, SdUndo *undo)
  {
  SdProject *prj = getSheet()->getProject();
  Q_ASSERT( prj != nullptr );

  //Realloc objects for this project
  mComponent = dynamic_cast<SdPItemComponent*>( prj->getFixedProjectItem(mComponent) );  //Object contains section information, pin assotiation info. May be same as mSymbol.
  mSymbol = dynamic_cast<SdPItemSymbol*>( prj->getFixedProjectItem(mSymbol) );        //Symbol contains graph information
  mPart = dynamic_cast<SdPItemPart*>( prj->getFixedProjectItem(mPart) );

  //Apply all items (symbol, component, part and partImp)
  Q_ASSERT( mSymbol != nullptr && mComponent != nullptr );

  undo->symImpPins( &mPins );

  if( mPart == nullptr ) {
    //Get part where this section resides
    int section = -1;

    //Link to part
    SdSection *sectionPtr = mComponent->getSection( section );
    if( sectionPtr == nullptr ) {
      mLinkError = QObject::tr("No pack info in %1 for section %2").arg(mComponent->getTitle()).arg(section+1);
      return;
      }

    //Accumulate pins
    mSymbol->forEach( dctSymPin, [this,sectionPtr] (SdObject *obj) -> bool {
      SdGraphSymPin *pin = dynamic_cast<SdGraphSymPin*>(obj);
      Q_ASSERT( pin != nullptr );

      QString pinName = pin->getPinName();
      //Test, if pinName duplicate
      if( mPins.contains(pinName) )
        pinName = QObject::tr("Duplicated pin name %1").arg(pinName);
      //Create implement pin
      SdSymImpPin impPin;
      impPin.mPin = pin;
      impPin.mPinNumber = sectionPtr->getPinNumber( pinName );

      //Add pin to table
      mPins.insert( pinName, impPin );
      return true;
      });
    }
  else {
    //Get part where this section resides
    int section = -1;
    SdGraphPartImp *partImp = plate->allocPartImp( &section, mPart, mComponent, mSymbol, mParamTable, undo );
    Q_ASSERT( partImp != nullptr );

    //Link to part
    undo->linkSection( section, this, partImp, true );
    partImp->setLinkSection( section, this );
    setLinkSection( section, partImp );
    SdSection *sectionPtr = mComponent->getSection( section );
    if( sectionPtr == nullptr ) {
      mLinkError = QObject::tr("No pack info in %1 for section %2").arg(mComponent->getTitle()).arg(section+1);
      return;
      }

    mPartImp->savePins( undo );
    //Accumulate pins
    mSymbol->forEach( dctSymPin, [this,sectionPtr] (SdObject *obj) -> bool {
      SdGraphSymPin *pin = dynamic_cast<SdGraphSymPin*>(obj);
      Q_ASSERT( pin != nullptr );

      QString pinName = pin->getPinName();
      //Test, if pinName duplicate
      if( mPins.contains(pinName) )
        pinName = QObject::tr("Duplicated pin name %1").arg(pinName);
      //Create implement pin
      SdSymImpPin impPin;
      impPin.mPin = pin;
      impPin.mPinNumber = sectionPtr->getPinNumber( pinName );

      //Link pins between symImp and partImp
      if( !mPartImp->partPinLink( impPin.mPinNumber, this, pinName, true ) )
        impPin.mPinNumber.clear();

      //Add pin to table
      mPins.insert( pinName, impPin );
      return true;
      });
    }

  updatePinsPositions();

  updatePinsConnectionStatus( undo );
  }




//Pin connection-disconnection by index for symbol and part implements
void SdGraphSymImp::pinConnectionSet( const QString pinName, const QString netName, SdUndo *undo)
  {
  Q_ASSERT( mPins.contains(pinName) );
  //Undo previous state of pin
  undo->pinSymImpStatus( this, pinName );
  //Set symbol pin connection state
  mPins[pinName].mNetName = netName;
  //Report to part to rebuild rat net
  if( mPartImp != nullptr )
    mPartImp->setDirtyRatNet();
  }






//Update connection status for all pins
void SdGraphSymImp::updatePinsConnectionStatus(SdUndo *undo)
  {
  SdPItemSheet *sheet = getSheet();
  Q_ASSERT( sheet != nullptr );
  QString netName;
  for( SdSymImpPinTable::const_iterator i = mPins.constBegin(); i != mPins.constEnd(); i++ )
    if( !i.value().isConnected() ) {
      //If pin unconnected then check possibilities connection
      if( sheet->getNetFromPoint( i.value().mPosition, netName ) )
        //New connection
        pinConnectionSet( i.key(), netName, undo );
//          else
//            //Disconnection
//            pinConnectionSet( i.key(), QString(), undo );
      }

  }











void SdGraphSymImp::attach(SdUndo *undo)
  {
  linkAutoPart( undo );
  }




//UnLink all items (symbol, component, part and partImp)
void SdGraphSymImp::detach(SdUndo *undo)
  {
  //Unlink from part
  unLinkPart( undo );
  //Autodelete all referenced objects
  if( mComponent ) mComponent->autoDelete( undo );
  if( mSymbol ) mSymbol->autoDelete( undo );
  if( mPart ) mPart->autoDelete( undo );
  }





void SdGraphSymImp::cloneFrom(const SdObject *src)
  {
  SdGraph::cloneFrom( src );
  const SdGraphSymImp *imp = dynamic_cast<const SdGraphSymImp*>( src );
  Q_ASSERT( imp != nullptr );
  mArea         = imp->mArea;        //PCB where this symbol implement contains in
  mSectionIndex = imp->mSectionIndex;//Section index (from 0)
  mLogSection   = imp->mLogSection;  //Logical symbol section number (from 1)
  mLogNumber    = imp->mLogNumber;   //Logical part number (from 1)
  mOrigin       = imp->mOrigin;      //Position of Implement
  mProp         = imp->mProp;        //Implement properties
  mOverRect     = imp->mOverRect;    //Over rect
  mIdentPrefix       = imp->mIdentPrefix;      //Part identificator prefix
  mIdentProp    = imp->mIdentProp;   //Part identificator text properties
  mIdentOrigin  = imp->mIdentOrigin; //Part identificator position in symbol context
  mIdentPos     = imp->mIdentPos;    //Part identificator position in sheet context
  mIdentRect    = imp->mIdentRect;   //Part identificator over rect

  mComponent    = imp->mComponent;   //Object contains section information, pin assotiation info. May be same as mSymbol.
  mSymbol       = imp->mSymbol;      //Symbol contains graph information
  mPart         = imp->mPart;
  //PartImp and pins assigned when attached to schematic sheet
  //mPartImp      = imp->mPartImp;
  //mPins         = imp->mPins;        //Pin information table
  mParamTable        = imp->mParamTable;       //Parameters
  }





void SdGraphSymImp::writeObject(QJsonObject &obj) const
  {
  SdGraph::writeObject( obj );
  writePtr( mArea, QStringLiteral("Area"), obj );        //PCB where this symbol implement contains in
  obj.insert( QStringLiteral("SectionIndex"), mSectionIndex );//Section index (from 0)
  obj.insert( QStringLiteral("LogSection"), mLogSection );    //Logical symbol section number (from 1)
  obj.insert( QStringLiteral("LogNumber"), mLogNumber );   //Logical part number (from 1)
  mOrigin.write( QStringLiteral("Org"), obj );      //Position of Implement
  mProp.write( obj );        //Implement properties
  mOverRect.write( QStringLiteral("Over"), obj );    //Over rect
  obj.insert( QStringLiteral("Prefix"), mIdentPrefix );      //Part identificator prefix
  mIdentProp.write( QStringLiteral("Ident"), obj );   //Part identificator text properties
  mIdentOrigin.write( QStringLiteral("IdentOrg"), obj ); //Part identificator position in symbol context
  mIdentPos.write( QStringLiteral("IdentPos"), obj );    //Part identificator position in sheet context
  mIdentRect.write( QStringLiteral("IdentOver"), obj );   //Part identificator over rect

  writePtr( mComponent, QStringLiteral("Comp"), obj );   //Object contains section information, pin assotiation info. May be same as mSymbol.
  writePtr( mSymbol, QStringLiteral("Sym"), obj );      //Symbol contains graph information
  writePtr( mPart, QStringLiteral("Part"), obj );
  writePtr( mPartImp, QStringLiteral("Imp"), obj );
  //Pin information table
  QJsonArray pins;
  for( SdSymImpPinTable::const_iterator i = mPins.constBegin(); i != mPins.constEnd(); i++ )
    pins.append( i.value().toJson( i.key() ) );
  obj.insert( QStringLiteral("Pins"), pins );
  //Parameters
  sdStringMapWrite( QStringLiteral("Param"), mParamTable, obj );
  }





void SdGraphSymImp::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mArea = dynamic_cast<SdGraphArea*>( readPtr( QStringLiteral("Area"), map, obj )  );
  mSectionIndex = obj.value( QStringLiteral("SectionIndex") ).toInt();
  mLogSection = obj.value( QStringLiteral("LogSection") ).toInt();
  mLogNumber = obj.value( QStringLiteral("LogNumber") ).toInt();
  mOrigin.read( QStringLiteral("Org"), obj );
  mProp.read( obj );
  mOverRect.read( QStringLiteral("Over"), obj );
  mIdentPrefix = obj.value( QStringLiteral("Prefix") ).toString();
  mIdentProp.read( QStringLiteral("Ident"), obj );   //Part identificator text properties
  mIdentOrigin.read( QStringLiteral("IdentOrg"), obj ); //Part identificator position in symbol context
  mIdentPos.read( QStringLiteral("IdentPos"), obj );    //Part identificator position in sheet context
  mIdentRect.read( QStringLiteral("IdentOver"), obj );   //Part identificator over rect

  mComponent = dynamic_cast<SdPItemComponent*>( readPtr( QStringLiteral("Comp"), map, obj )  );
  mSymbol = dynamic_cast<SdPItemSymbol*>( readPtr( QStringLiteral("Sym"), map, obj )  );
  mPart = dynamic_cast<SdPItemPart*>( readPtr( QStringLiteral("Part"), map, obj )  );
  mPartImp = dynamic_cast<SdGraphPartImp*>( readPtr( QStringLiteral("Imp"), map, obj )  );

  //Pin information table
  QJsonArray pins = obj.value( QStringLiteral("Pins") ).toArray();
  for( QJsonValue vpin : pins ) {
    SdSymImpPin pin;
    QString pinName = pin.fromJson( map, vpin.toObject() );
    mPins.insert( pinName, pin );
    }
  //Parameters
  sdStringMapRead( QStringLiteral("Param"), mParamTable, obj );
  }





bool SdGraphSymImp::isUsed(SdObject *obj) const
  {
  return obj == mComponent || obj == mSymbol || obj == mPart;
  }





void SdGraphSymImp::upgradeProjectItem(SdProjectItem *newItem, SdUndo *undo)
  {
  if( mPart != nullptr && mPart->isCanUpgaded(newItem) ) {
    //Part upgrade
    detach(undo);
    mPart = dynamic_cast<SdPItemPart*>(newItem);
    attach(undo);
    }
  else if( mComponent->isCanUpgaded(newItem) ) {
    //Component upgrade and may be symbol
    detach(undo);
    mComponent = dynamic_cast<SdPItemComponent*>(newItem);
    if( mSymbol->isCanUpgaded(newItem) )
      mSymbol = dynamic_cast<SdPItemSymbol*>(newItem);
    attach(undo);
    }
  else if( mSymbol->isCanUpgaded(newItem) ) {
    //Only symbol upgrade
    detach(undo);
    mSymbol = dynamic_cast<SdPItemSymbol*>(newItem);
    attach(undo);
    }
  }

