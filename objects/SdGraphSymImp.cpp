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
#include "SdContainerSheetNet.h"
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

SdGraphSymImp::SdGraphSymImp(SdPItemSymbol *comp, SdPItemSymbol *sym, SdPItemPart *part, SdPoint pos, SdPropSymImp *prp ) :
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
    SdConverterImplement imp( mOrigin, sym->getOrigin(), mProp.mAngle.getValue(), mProp.mMirror.getValue() );
    QTransform t( imp.getMatrix() );
    mOverRect.set( t.mapRect(sym->getOverRect()) );//Over rect
    mPrefix = sym->getIdent()->getText();          //Part identificator prefix
    mIdentProp = sym->getIdent()->getPropText();   //Part identificator text properties
    mIdentOrigin = sym->getIdent()->getOrigin();   //Part identificator position in symbol context
    mIdentPos = t.map( mIdentOrigin );    //Part identificator position in sheet context
    mIdentRect.set( t.mapRect( sym->getIdent()->getOverRect() )  );   //Part identificator over rect
    }
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
    return mPins.value( pinName ).mWireName;
  return QString();
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
  //TODO D013 make BOM
  return QString();
#if 0
  QString bom = mParam.value( QStringLiteral("bom") ).toString();
  CPChar sour;
  if( param.GetBuffer() != NULL ) sour = strstr( param.GetBuffer(), "Перечень=" );
  else return false; //Не заносим в перечень неопределенный компонент
  char buf[1000];
  char *dest = buf;
  int c = 0;
  if( sour ) {
    sour += 9; //Пропустить слово Перечень=
    while( c < 999 && *sour && *sour != '\n' && *sour != '\r' ) {
      if( *sour == '<' ) {
        sour++; //Пропустить открывающую скобку
        //Сканировать имя
        char tmpName[260];
        int i;
        for( i = 0; i < 259 && *sour && *sour != '\n' && *sour != '>'; ++i ) {
          tmpName[i] = *sour++;
          }
        tmpName[i] = 0; //Закрыть имя
        if( *sour == '>' ) sour++; //Пропустить закрывающую скобку
        else break;
        //Обнаружить параметр, соответствующий имени
        strcat( tmpName, "=" );
        CPChar ptr = strstr( param.GetBuffer(), tmpName );
        if( ptr ) {
          ptr += strlen( tmpName );
          //Скопировать значение параметра
          for( ; c < 999 && *ptr && *ptr != '\n' && *ptr != '\r'; ++c )
            *dest++ = *ptr++;
          }
        }
      else {
        c++;
        *dest++ = *sour++;
        }
      }
    }
  else return false; //Не заносим в перечень неопределенный компонент
  *dest = 0; //Закрываем сформированную строку
  def = buf;
  return true;
#endif
  }




//Get full visual ident of section aka D4.2
QString SdGraphSymImp::getIdent() const
  {
  if( mLogSection )
    return QString("%1%2.%3").arg( mPrefix ).arg( mLogNumber ).arg( mLogSection );
  return QString("%1%2").arg( mPrefix ).arg( mLogNumber );
  }




//Get separated ident information
QString SdGraphSymImp::getIdentInfo(int &logNumber, int &logSection)
  {
  logNumber = mLogNumber;
  logSection = mLogSection;
  return mPrefix;
  }




//Set ident information
void SdGraphSymImp::setIdentInfo(const QString prefix, int logNumber, int logSection)
  {
  mPrefix = prefix;
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
void SdGraphSymImp::netWirePlace(SdPoint a, SdPoint b, const QString name, SdUndo *undo)
  {
  for( SdSymImpPinTable::const_iterator i = mPins.constBegin(); i != mPins.constEnd(); i++ )
    if( i.value().isCanConnect( a, b ) )
      //Set new state of pin
      pinConnectionSet( i.key(), name, undo );
  }




//Notification about wire segment deletion
void SdGraphSymImp::netWireDelete(SdPoint a, SdPoint b, const QString name, SdUndo *undo)
  {
  for( SdSymImpPinTable::const_iterator i = mPins.constBegin(); i != mPins.constEnd(); i++ )
    if( i.value().isCanDisconnect( a, b, name ) )
      //Set new state of pin
      pinConnectionSet( i.key(), QString(), undo );
  }




//Accumulate segments connected to component
void SdGraphSymImp::accumLinked(SdPoint a, SdPoint b, SdSelector *sel)
  {
  //Scan all pins and check if segment ab connected to any pin then select component
  for( SdSymImpPin &pin : mPins )
    if( pin.isConnected() && pin.mPosition.isOnSegment(a,b) ) {
      sel->insert( this );
      return;
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
  undo->symImp( &mOrigin, &mProp, &mLogSection, &mLogNumber, &mOverRect, &mPrefix, &mIdentProp, &mIdentOrigin, &mIdentPos, &mIdentRect );
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
      pin.prepareMove( sheet, sel );
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
      info = getBomItemLine();
      if( !info.isEmpty() )
        return true;
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
  mOverRect.set( t.mapRect( mSymbol->getOverRect() ) );
  mIdentPos = t.map( mIdentOrigin );
  //mIdentRect.set( )
  }








//Link auto partImp in given plate. partImp and section are selected automatic
void SdGraphSymImp::linkAutoPartInPlate(SdPItemPlate *plate, SdUndo *undo)
  {
  SdProject *prj = getSheet()->getProject();
  Q_ASSERT( prj != nullptr );

  //Realloc objects for this project
  mComponent = dynamic_cast<SdPItemSymbol*>( prj->getProjectsItem(mComponent) );  //Object contains section information, pin assotiation info. May be same as mSymbol.
  mSymbol = dynamic_cast<SdPItemSymbol*>( prj->getProjectsItem(mSymbol) );        //Symbol contains graph information
  mPart = dynamic_cast<SdPItemPart*>( prj->getProjectsItem(mPart) );

  //Apply all items (symbol, component, part and partImp)
  Q_ASSERT( mPins.count() == 0 && mSymbol != nullptr && mComponent != nullptr );

  //Ensure all components are fixed
  if( mSymbol->isEditEnable() ) {
    mLinkError = QObject::tr("Symbol %1 in editing state").arg(mSymbol->getTitle());
    return;
    }
  if( mComponent->isEditEnable() ) {
    mLinkError = QObject::tr("Component %1 in editing state").arg(mComponent->getTitle());
    return;
    }
  if( mPart != nullptr && mPart->isEditEnable() ) {
    mLinkError = QObject::tr("Part %1 in editing state").arg(mPart->getTitle());
    return;
    }

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
    mSymbol->forEach( dctSymPin, [this,sectionPtr,undo] (SdObject *obj) -> bool {
      SdGraphSymPin *pin = dynamic_cast<SdGraphSymPin*>(obj);
      Q_ASSERT( pin != nullptr );

      QString pinName = pin->getPinName();
      //Test, if pinName duplicate
      if( mPins.contains(pinName) )
        pinName = QObject::tr("Duplicated pin name %1").arg(pinName);
      //Create implement pin
      SdSymImpPin impPin;
      impPin.mPin = pin;

      //Add pin to table
      mPins.insert( pinName, impPin );
      return true;
      });
    }
  else {
    //Get part where this section resides
    int section = -1;
    SdGraphPartImp *partImp = plate->allocPartImp( &section, mPart, mComponent, mSymbol, undo );
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
    mSymbol->forEach( dctSymPin, [this,sectionPtr,undo] (SdObject *obj) -> bool {
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
  mPins[pinName].mWireName = netName;
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
  mPrefix       = imp->mPrefix;      //Part identificator prefix
  mIdentProp    = imp->mIdentProp;   //Part identificator text properties
  mIdentOrigin  = imp->mIdentOrigin; //Part identificator position in symbol context
  mIdentPos     = imp->mIdentPos;    //Part identificator position in sheet context
  mIdentRect    = imp->mIdentRect;   //Part identificator over rect

  mComponent    = imp->mComponent;   //Object contains section information, pin assotiation info. May be same as mSymbol.
  mSymbol       = imp->mSymbol;      //Symbol contains graph information
  mPart         = imp->mPart;
  mPartImp      = imp->mPartImp;
  mPins         = imp->mPins;        //Pin information table
  mParam        = imp->mParam;       //Parameters
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
  obj.insert( QStringLiteral("Prefix"), mPrefix );      //Part identificator prefix
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
  sdParamWrite( QStringLiteral("Param"), mParam, obj );
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
  mPrefix = obj.value( QStringLiteral("Prefix") ).toString();
  mIdentProp.read( QStringLiteral("Ident"), obj );   //Part identificator text properties
  mIdentOrigin.read( QStringLiteral("IdentOrg"), obj ); //Part identificator position in symbol context
  mIdentPos.read( QStringLiteral("IdentPos"), obj );    //Part identificator position in sheet context
  mIdentRect.read( QStringLiteral("IdentOver"), obj );   //Part identificator over rect

  mComponent = dynamic_cast<SdPItemSymbol*>( readPtr( QStringLiteral("Comp"), map, obj )  );
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
  sdParamRead( QStringLiteral("Param"), mParam, obj );
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
    mComponent = dynamic_cast<SdPItemSymbol*>(newItem);
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

