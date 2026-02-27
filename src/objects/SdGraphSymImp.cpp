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
#include "SdGraphValue.h"
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
#include "library/SdLibraryStorage.h"
#include "SdJsonIO.h"

#include <QDebug>



//====================================================================================
//Symbol implementation
SdGraphSymImp::SdGraphSymImp() :
  SdGraphParam(),
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

SdGraphSymImp::SdGraphSymImp(SdPItemVariant *comp, SdPItemSymbol *sym, SdPItemPart *part, const SdStringMap &param, SdPoint pos, SdPropSymImp *prp ) :
  SdGraphParam(param),
  mArea(nullptr),        //PCB where this symbol implement contains in
  mSectionIndex(0),      //Section index (from 0)
  mLogSection(0),        //Logical symbol section number (from 1)
  mLogNumber(0),         //Logical part number (from 1)
  mOrigin(pos),          //Position of Implement
  mComponent(comp),      //Object contains section information, pin assotiation info. May be same as mSymbol.
  mSymbol(sym),          //Symbol contains graph information
  mPart(part),
  mPartImp(nullptr)
  {
  //QString           mName;        //Name of component
  mProp = *prp;        //Implement properties
  if( sym ) {
    QTransform t( matrix() );
    mOverRect.set( t.mapRect(sym->getOverRect(dctAll & (~(dctIdent | dctValue | dctSymPin)))) );//Over rect
    mIdent.mProp = sym->identGet()->getPropText();   //Symbol identificator text properties
    mIdent.mOrigin = sym->identGet()->getOrigin();   //Symbol identificator position in part context
    mValue.mProp = sym->valueGet()->getPropText();   //Symbol value text properties
    mValue.mOrigin = sym->valueGet()->getOrigin();   //Symbol value position in part context
    }
  }

QTransform SdGraphSymImp::matrix() const
  {
  SdConverterImplement imp( mOrigin, mSymbol->getOrigin(), mProp.mAngle, mProp.mMirror.asBool() );
  return imp.getMatrix();
  }





//Retrive origin point and sheet number
QString SdGraphSymImp::getRenumSect(SdPoint &dest, int &sheetNumber) const
  {
  dest = mOverRect.getTopLeft();
  sheetNumber = getSheet()->getSheetIndex();
  return identPrefix();
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
    undo->prop( &mParamTable );

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




//Unconnect pin over rect
void SdGraphSymImp::unconnectPinOverRect(SdRect over, SdUndo *undo, const QString undoTitle)
  {
  bool first = true;
  for( auto i = mPins.constBegin(); i != mPins.constEnd(); i++ )
    if( over.isPointInside( i.value().mPosition ) ) {
      if( undo && first ) {
        first = false;
        undo->begin( undoTitle, getSheet(), false );
        }
      //Set new state of pin
      pinConnectionSet( i.key(), QString(), undo );
      }
  }






void SdGraphSymImp::unLinkPart(SdUndo *undo)
  {
  if( mPartImp != nullptr ) {
    //Save pin state
    undo->prop( &mPins );
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




//Get ident prefix
QString SdGraphSymImp::identPrefix() const
  {
  return paramGet( stdParamPrefix );
  }




//Get full visual ident of section aka D4.2
QString SdGraphSymImp::ident() const
  {
  if( mLogSection )
    return QString("%1%2.%3").arg( identPrefix() ).arg( mLogNumber ).arg( mLogSection );
  return QString("%1%2").arg( identPrefix() ).arg( mLogNumber );
  }




//Set ident text properties and position
void SdGraphSymImp::identSet(const SdPropText &prp, SdPoint pos, SdUndo *undo)
  {
  if( undo )
    undo->prop( &mIdent.mProp, &mIdent.mOrigin );
  mIdent.mProp = prp;
  mIdent.mOrigin = pos;
  }








//Get separated ident information
QString SdGraphSymImp::identInfoGet(int &logNumber, int &logSection)
  {
  logNumber = mLogNumber;
  logSection = mLogSection;
  return identPrefix();
  }




//Set ident information
void SdGraphSymImp::identInfoSet( int logNumber, int logSection)
  {
  mLogNumber = logNumber;
  mLogSection = logSection;
  }





//Get full visual value of part aka smt32f417vgt
QString SdGraphSymImp::value() const
  {
  return paramGet( stdParamValue );
  }





//Set value text properties and position
void SdGraphSymImp::valueSet(const SdPropText &prp, SdPoint pos, SdUndo *undo)
  {
  if( undo )
    undo->prop( &mValue.mProp, &mValue.mOrigin );
  mValue.mProp = prp;
  mValue.mOrigin = pos;
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
  //Save state of all properties and pins
  undo->prop( &mOrigin, &mProp, &mLogSection, &mLogNumber, &mOverRect, &mPins );
  }




void SdGraphSymImp::moveComplete(SdPoint grid, SdUndo *undo)
  {
  Q_UNUSED(grid)
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




void SdGraphSymImp::transform(const QTransform &map, SdPvAngle angle)
  {
  mOrigin = map.map(mOrigin);
  mProp.mAngle += angle;
  updatePinsPositions();
  }






void SdGraphSymImp::setProp(SdPropSelected &prop)
  {
  if( prop.mSymImpProp.store( mProp ) ) {
    //Unconnect all pins from wires
    //ucomAllPins();
    //setup props
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
  //Convertor for symbol implementation
  SdConverterImplement imp( mOrigin, mSymbol->getOrigin(), mProp.mAngle, mProp.mMirror.asBool() );
  dc->setConverter( &imp );

  //Draw ident in symbol context
  SdRect ov;
  dc->text( mIdent.mOrigin, ov, ident(), mIdent.mProp );
  dc->text( mValue.mOrigin, ov, value(), mValue.mProp );

  dc->paramMapSet( &mParamTable );

  //Draw symbol except ident and pins
  mSymbol->forEach( dctAll & ~(dctSymPin | dctIdent | dctValue), [dc] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph )
      graph->draw( dc );
    return true;
    });

  //Draw pins
  for( SdSymImpPin &pin : mPins )
    pin.draw( dc );

  dc->paramMapSet();
  }




int SdGraphSymImp::behindCursor(SdPoint p)
  {
  if( mOverRect.isPointInside(p) )
    return getSelector() ? ELEM_SEL : ELEM_UNSEL;
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
        info = ident() + QString("  ") + bom;
        return true;
        }
      }
    info = ident();
    if( mComponent )
      info.append( QString("  ") ).append( mComponent->getTitle() );
    return true;
    }
  return false;
  }





//Find snap point on object
void SdGraphSymImp::snapPoint(SdSnapInfo *snap)
  {
  if( snap->match(snapNextPin) ) {
    //Must be checking with direction control
    for( SdSymImpPin &pin : mPins )
      if( !pin.isConnected() && calcDirection90( snap->mSour, pin.mPosition ) == snap->mDir )
        snap->test( this, pin.mPosition, snapNextPin );
    }
  if( snap->match(snapNearestPin|snapNearestNet) ) {
    //Checked without direction control
    for( SdSymImpPin &pin : mPins )
      if( !pin.isConnected() )
        snap->test( this, pin.mPosition, snapNearestPin );
    }
  }




SdPItemSheet *SdGraphSymImp::getSheet() const
  {
  return dynamic_cast<SdPItemSheet*>( getParent() );
  }




void SdGraphSymImp::updatePinsPositions()
  {
  SdConverterImplement impl( mOrigin, mSymbol->getOrigin(), mProp.mAngle, mProp.mMirror.asBool() );
  QTransform t = impl.getMatrix();
  for( SdSymImpPin &pin : mPins )
    pin.mPosition = t.map( pin.mPin->getPinOrigin() );
  mOverRect.set( t.mapRect( mSymbol->getOverRect(dctAll & (~(dctIdent | dctValue | dctSymPin))) ) );
  }








//Link auto partImp in given plate. partImp and section are selected automatic
void SdGraphSymImp::linkAutoPartInPlate(SdPItemPlate *plate, SdUndo *undo)
  {
  SdProject *prj = getSheet()->getProject();
  Q_ASSERT( prj != nullptr );

  //Realloc objects for this project
  mComponent = dynamic_cast<SdPItemVariant*>( prj->getFixedProjectItem(mComponent) );  //Object contains section information, pin assotiation info. May be same as mSymbol.
  mSymbol = dynamic_cast<SdPItemSymbol*>( prj->getFixedProjectItem(mSymbol) );        //Symbol contains graph information
  mPart = dynamic_cast<SdPItemPart*>( prj->getFixedProjectItem(mPart) );

  //Apply all items (symbol, component, part and partImp)
  Q_ASSERT( mSymbol != nullptr && mComponent != nullptr );

  undo->prop( &mPins );

  if( mPart == nullptr ) {
    return;
    //Get part where this section resides
    int section = -1;

    //Link to part
    if( !mComponent->sectionIsAvailable(section) ) {
      mLinkError = QObject::tr("No pack info in %1 for section %2").arg(mComponent->getTitle()).arg(section+1);
      return;
      }

    //Accumulate pins
    mSymbol->forEach( dctSymPin, [this,section] (SdObject *obj) -> bool {
      SdGraphSymPin *pin = dynamic_cast<SdGraphSymPin*>(obj);
      Q_ASSERT( pin != nullptr );

      QString pinName = pin->getPinName();
      //Test, if pinName duplicate
      if( mPins.contains(pinName) )
        pinName = QObject::tr("Duplicated pin name %1").arg(pinName);
      //Create implement pin
      SdSymImpPin impPin;
      impPin.mPin = pin;
      impPin.mPinNumber = mComponent->sectionPinNumberGet( section, pinName );

      //Add pin to table
      mPins.insert( pinName, impPin );
      return true;
      });
    }
  else {
    //Get part where this section resides
    int section = -1;
    SdGraphPartImp *partImp = plate->allocPartImp( &section, mPart, mComponent, mSymbol, mParamTable, undo );
    //qDebug() << "Allocated section" << section;
    Q_ASSERT( partImp != nullptr );

    //Link to part
    undo->linkSection( section, this, partImp, true );
    partImp->setLinkSection( section, this );
    setLinkSection( section, partImp );
    if( !mComponent->sectionIsAvailable(section) ) {
      mLinkError = QObject::tr("No pack info in %1 for section %2").arg(mComponent->getTitle()).arg(section+1);
      return;
      }

    mPartImp->savePins( undo );
    //Accumulate pins
    mSymbol->forEach( dctSymPin, [this,section] (SdObject *obj) -> bool {
      SdGraphSymPin *pin = dynamic_cast<SdGraphSymPin*>(obj);
      Q_ASSERT( pin != nullptr );

      QString pinName = pin->getPinName();
      //Test, if pinName duplicate
      if( mPins.contains(pinName) )
        pinName = QObject::tr("Duplicated pin name %1").arg(pinName);
      //Create implement pin
      SdSymImpPin impPin;
      impPin.mPin = pin;
      impPin.mPinNumber = mComponent->sectionPinNumberGet( section, pinName );

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





//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphSymImp::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraphParam::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphSymImp> imp( src );
  Q_ASSERT( imp.isValid() );
  mArea         = imp->mArea;        //PCB where this symbol implement contains in
  mSectionIndex = imp->mSectionIndex;//Section index (from 0)
  mLogSection   = imp->mLogSection;  //Logical symbol section number (from 1)
  mLogNumber    = imp->mLogNumber;   //Logical part number (from 1)
  mOrigin       = imp->mOrigin;      //Position of Implement
  mProp         = imp->mProp;        //Implement properties
  mOverRect     = imp->mOverRect;    //Over rect
  mIdent        = imp->mIdent;
  mValue        = imp->mValue;

  mComponent    = imp->mComponent;   //Object contains section information, pin assotiation info. May be same as mSymbol.
  mSymbol       = imp->mSymbol;      //Symbol contains graph information
  mPart         = imp->mPart;
  //PartImp and pins assigned when attached to schematic sheet
  //mPartImp      = imp->mPartImp;
  //mPins         = imp->mPins;        //Pin information table
  }




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdGraphSymImp::json(SdJsonWriter &js) const
  {
  //PCB where this symbol implement contains in
  js.jsonObjectPtr( QStringLiteral("Area"), mArea );

  //Section index (from 0)
  js.jsonInt( QStringLiteral("SectionIndex"), mSectionIndex );

  //Logical symbol section number (from 1)
  js.jsonInt( QStringLiteral("LogSection"), mLogSection );

  //Logical part number (from 1)
  js.jsonInt( QStringLiteral("LogNumber"), mLogNumber );

  //Position of Implement
  js.jsonPoint( QStringLiteral("Org"), mOrigin );

  //Implement properties
  mProp.json( js );

  //Over rect
  js.jsonRect( QStringLiteral("Over"), mOverRect );
  mIdent.json( QStringLiteral("Ident"), js );
  mValue.json( QStringLiteral("Value"), js );

  //Object contains section information, pin assotiation info. May be same as mSymbol.
  js.jsonObjectPtr( QStringLiteral("Comp"), mComponent );
  //Symbol contains graph information
  js.jsonObjectPtr( QStringLiteral("Sym"), mSymbol );
  js.jsonObjectPtr( QStringLiteral("Part"), mPart );
  js.jsonObjectPtr( QStringLiteral("Imp"), mPartImp );

  //Pin information table
  js.jsonMap( js, QStringLiteral("Pins:"), mPins );

  SdGraphParam::json( js );
  }



//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdGraphSymImp::json(const SdJsonReader &js)
  {
  //PCB where this symbol implement contains in
  js.jsonObjectPtr( QStringLiteral("Area"), mArea );

  //Section index (from 0)
  js.jsonInt( QStringLiteral("SectionIndex"), mSectionIndex );

  //Logical symbol section number (from 1)
  js.jsonInt( QStringLiteral("LogSection"), mLogSection );

  //Logical part number (from 1)
  js.jsonInt( QStringLiteral("LogNumber"), mLogNumber );

  //Position of Implement
  js.jsonPoint( QStringLiteral("Org"), mOrigin );

  //Implement properties
  mProp.json( js );

  //Over rect
  js.jsonRect( QStringLiteral("Over"), mOverRect );
  mIdent.json( QStringLiteral("Ident"), js );
  mValue.json( QStringLiteral("Value"), js );

  //Object contains section information, pin assotiation info. May be same as mSymbol.
  js.jsonObjectPtr( QStringLiteral("Comp"), mComponent );
  //Symbol contains graph information
  js.jsonObjectPtr( QStringLiteral("Sym"), mSymbol );
  js.jsonObjectPtr( QStringLiteral("Part"), mPart );
  js.jsonObjectPtr( QStringLiteral("Imp"), mPartImp );

  //Pin information table
  if( js.contains( QStringLiteral("Pins") ) ) {
    //Pins as array
    QJsonArray pins = js.object().value( QStringLiteral("Pins") ).toArray();
    for( const QJsonValue &vpin : std::as_const(pins) ) {
      SdSymImpPin pin;
      SdJsonReader pjs( vpin.toObject(), js );
      QString pinName = pin.fromJson( pjs );
      mPins.insert( pinName, pin );
      }
    }
  else
    js.jsonMap( js, QStringLiteral("Pins:"), mPins );

  SdGraphParam::json( js );
  }







bool SdGraphSymImp::isUsed(SdObject *obj) const
  {
  return obj == mComponent || obj == mSymbol || obj == mPart;
  }






bool SdGraphSymImp::upgradeProjectItem(SdUndo *undo, QWidget *parent)
  {
  Q_UNUSED(parent)
  if( SdLibraryStorage::instance()->isLibraryObjectPresentAndNewer(mComponent) || SdLibraryStorage::instance()->isLibraryObjectPresentAndNewer(mSymbol) || SdLibraryStorage::instance()->isLibraryObjectPresentAndNewer(mPart) ) {
    //There newer objects. Upgrade.

    //Prepare newer objects
    QScopedPointer<SdPItemComponent> comp( sdObjectOnly<SdPItemComponent>( mComponent ? SdLibraryStorage::instance()->cfObjectGet( mComponent->hashUidName() ) : nullptr ) );
    QScopedPointer<SdPItemSymbol>    sym( sdObjectOnly<SdPItemSymbol>( comp ? comp->extractSymbolFromFactory( mSectionIndex ) : nullptr ) );
    QScopedPointer<SdPItemPart>      part( sdObjectOnly<SdPItemPart>( comp ? comp->partExtractFromFactory() : nullptr ) );

    //Test if all newer objects prepared
    if( !comp.isNull() && !sym.isNull() && !part.isNull() ) {
      //Retrive implement properties for restore position and other properties of implement because after
      //upgrading mPartImp will be different from current
      SdPropSelected prop;
      prop.clear();
      SdPoint partOrigin( SdPoint::farPoint() );
      if( mPartImp ) {
        mPartImp->getProp( prop );
        partOrigin = mPartImp->getOrigin();
        }
      detach(undo);
      //Because "detach" perhaps without deleting symImp, then referenced object not autodeleted
      //Perform autodeling it
      SdPItemVariant   *dcomp = mComponent;
      SdPItemSymbol    *dsym  = mSymbol;
      SdPItemPart      *dpart = mPart;

      //This assignes are temporary, it will be replace on actual in attach function
      mComponent = comp.get();
      mSymbol    = sym.get();
      mPart      = part.get();

      //Replace params which has no variants
      //B065 When upgrading component don't changed ident (and perhaps - value)
      SdStringMap newMap;
      for( auto iter = mParamTable.cbegin(); iter != mParamTable.cend(); iter++ )
        if( mComponent->isFieldPresent( iter.key() ) || !mComponent->paramContains( iter.key() ) )
          //If field present in variant table then leave field value
          newMap.insert( iter.key(), iter.value() );
        else
          //Replace field
          newMap.insert( iter.key(), mComponent->paramGet( iter.key() ) );

      //Autodelete all referenced objects
      if( dcomp ) dcomp->autoDelete( undo );
      if( dsym  ) dsym->autoDelete( undo );
      if( dpart ) dpart->autoDelete( undo );

      //At this point mComponent, mSymbol and mPart will be replaced with project owner instances
      attach(undo);
      //Restore part implement properties
      if( mPartImp ) {
        mPartImp->setProp( prop );
        if( !partOrigin.isFar() ) {
          partOrigin = partOrigin.sub( mPartImp->getOrigin() );
          mPartImp->transform( partOrigin.transformFromOffset(), SdPvAngle{} );
          }
        }

      //Setup new param table
      paramTableSet( newMap, undo, nullptr );

      return true;
      }
    return false;
    }
  return true;
  }






