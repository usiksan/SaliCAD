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
#include <QDebug>


//====================================================================================
//Pin for symbol implementation

SdSymImpPin::SdSymImpPin() :
  mPin(0),       //Pin
  mCom(false),   //State of pin to net connectivity
  mPrtPin(-1)    //Pin index in pin array of part implement
  {

  }



void SdSymImpPin::operator =(const SdSymImpPin &pin)
  {
  mPin       = pin.mPin;       //Pin
  mPinName   = pin.mPinName;   //Pin name in symbol
  mPinNumber = pin.mPinNumber; //Pin number in part
  mPosition  = pin.mPosition;  //Pin position in sheet context
  mWireName  = pin.mWireName;  //Net, which pin connected to
  mCom       = pin.mCom;       //State of pin to net connectivity
  mPrtPin    = pin.mPrtPin;    //Pin index in pin array of part implement
  }



void SdSymImpPin::draw(SdContext *dc)
  {
  mPin->drawImp( dc, mPinNumber, mCom );
  }




void SdSymImpPin::setConnection(const QString wireName, bool com)
  {
  mCom = com;
  if( mCom )
    mWireName = wireName;
  }

void SdSymImpPin::setConnection(SdGraphPartImp *partImp, const QString wireName, bool com)
  {
  mCom = com;
  if( mCom )
    mWireName = wireName;
  if( partImp )
    partImp->pinConnectionSet( mPrtPin, wireName, com );
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



QJsonObject SdSymImpPin::toJson() const
  {
  QJsonObject obj;
  SdObject::writePtr( mPin, QStringLiteral("Pin"), obj );
  obj.insert( QStringLiteral("Name"), mPinName );     //Pin name in symbol
  obj.insert( QStringLiteral("Number"), mPinNumber ); //Pin number in part
  mPosition.write( QStringLiteral("Pos"), obj );      //Pin position in sheet context
  obj.insert( QStringLiteral("Wire"), mWireName );    //Net, which pin connected to
  obj.insert( QStringLiteral("Com"), mCom );          //State of pin to net connectivity
  obj.insert( QStringLiteral("Prt"), mPrtPin );       //Pin index in pin array of part implement
  return obj;
  }




void SdSymImpPin::fromJson(SdObjectMap *map, const QJsonObject obj)
  {
  mPin = dynamic_cast<SdGraphSymPin*>( SdObject::readPtr( QStringLiteral("Pin"), map, obj ) );
  mPinName = obj.value( QStringLiteral("Name") ).toString();
  mPinNumber = obj.value( QStringLiteral("Number") ).toString();
  mPosition.read( QStringLiteral("Pos"), obj );
  mWireName = obj.value( QStringLiteral("Wire") ).toString();
  mCom = obj.value( QStringLiteral("Com") ).toBool();
  mPrtPin = obj.value( QStringLiteral("Prt") ).toInt();
  }





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
    SdConverterImplement imp( mOrigin, sym->mOrigin, mProp.mAngle.getValue(), mProp.mMirror.getValue() );
    QTransform t( imp.getMatrix() );
    mOverRect.set( t.mapRect(sym->getOverRect()) );//Over rect
    mPrefix = sym->getIdent()->getText();          //Part identificator prefix
    mIdentProp = sym->getIdent()->getPropText();   //Part identificator text properties
    mIdentOrigin = sym->getIdent()->getOrigin();   //Part identificator position in symbol context
    mIdentPos = t.map( mIdentOrigin );    //Part identificator position in sheet context
    mIdentRect.set( t.mapRect( sym->getIdent()->getOverRect() )  );   //Part identificator over rect
    }
  }




//Pin connection-disconnection by index
void SdGraphSymImp::pinConnectionSet(int pinIndex, const QString wireName, bool com)
  {
  if( pinIndex < 0 ) return;
  Q_ASSERT( pinIndex >= 0 && pinIndex < mPins.count() );
  mPins[pinIndex].setConnection( wireName, com );
  }




void SdGraphSymImp::moveToPlate(SdPItemPlate *plate, SdUndo *undo)
  {
  //UnLink part in current plate
  unLinkPartImp( undo );

  //And link to part in another plate
  linkAutoPartInPlate( plate, undo );
  }




void SdGraphSymImp::setLinkSection(int section, SdGraphPartImp *partImp)
  {
  //If symbol linked then unlink
  if( mPartImp != nullptr ) {
    //Scan all pins and disconnect from partImp
    for( int index = 0; index < mPins.count(); index++ ) {
      mPartImp->pinConnectionSet( mPins[index].mPrtPin, QString(), false );
      mPins[index].mPrtPin = -1;
      }
    mPartImp = nullptr;
    mSectionIndex = -1;
    }

  //If passed new one link part then link
  if( partImp != nullptr ) {
    SdSection *sct = mComponent->getSection( section );
    //Part unliked. Perform link
    for( int index = 0; index < mPins.count(); index++ ) {
      //Try get from section info pinNumber for pinName
      if( sct ) {
        mPins[index].mPinNumber = sct->getPinNumber( mPins[index].mPinName );
        //PinNumber received, get part pin index
        mPins[index].mPrtPin = partImp->getPinIndex( mPins[index].mPinNumber );
        //Assign pin connection
        partImp->pinConnectionSet( mPins[index].mPrtPin, mPins[index].mWireName, mPins[index].mCom );
        }
      else {
        //No pin number.
        mPins[index].mPinNumber.clear();
        mPins[index].mPrtPin = -1;
        }
      }
    mPartImp = partImp;
    mSectionIndex = section;
    }
  }




//Unconnect pin in point
void SdGraphSymImp::unconnectPinInPoint( SdPoint p, SdUndo *undo )
  {
  for( int index = 0; index < mPins.count(); index++ )
    if( mPins[index].mPosition == p ) {
      //Undo previous state of pin
      undo->pinImpConnect( this, index, mPartImp, mPins[index].mPrtPin, mPins[index].mWireName, mPins[index].mCom );
      //Set new state of pin
      mPins[index].setConnection( mPartImp, QString(), false );
      return;
      }
  }




void SdGraphSymImp::unLinkPartImp(SdUndo *undo)
  {
  //Save current link state
  undo->linkSection( mSectionIndex, this, mPartImp, mPartImp != nullptr );
  if( mPartImp )
    mPartImp->setLinkSection( mSectionIndex, nullptr );
  setLinkSection( -1, nullptr );
  }




//Link auto partImp. partImp and section are selected automatic
void SdGraphSymImp::linkAutoPart(SdUndo *undo)
  {
  //Get plate where component resides
  SdPItemPlate *plate = getSheet()->getPlate( mOrigin );
  Q_ASSERT( plate != nullptr );

  linkAutoPartInPlate( plate, undo );

  }




//Get wire name pin with pinIndex connected to
QString SdGraphSymImp::pinWireName(int pinIndex) const
  {
  Q_ASSERT( pinIndex >= 0 && pinIndex < mPins.count() );
  return mPins[pinIndex].mWireName;
  }




//Return if pin with pinIndex connected to any wire or not
bool SdGraphSymImp::isPinConnected(int pinIndex) const
  {
  Q_ASSERT( pinIndex >= 0 && pinIndex < mPins.count() );
  return mPins[pinIndex].mCom;
  }




//Get BOM item line
QString SdGraphSymImp::getBomItemLine() const
  {
  //TODO make BOM
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
  //TODO update origin regarding symbol
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
  for( int index = 0; index < mPins.count(); index++ )
    if( mPins[index].isCanConnect( a, b ) ) {
      //Undo previous state of pin
      undo->pinImpConnect( this, index, mPartImp, mPins[index].mPrtPin, mPins[index].mWireName, mPins[index].mCom );
      //Set new state of pin
      mPins[index].setConnection( mPartImp, name, true );
      }
  }




//Notification about wire segment deletion
void SdGraphSymImp::netWireDelete(SdPoint a, SdPoint b, const QString name, SdUndo *undo)
  {
  for( int index = 0; index < mPins.count(); index++ )
    if( mPins[index].isCanDisconnect( a, b, name ) ) {
      //Undo previous state of pin
      undo->pinImpConnect( this, index, mPartImp, mPins[index].mPrtPin, mPins[index].mWireName, mPins[index].mCom );
      //Set new state of pin
      mPins[index].setConnection( mPartImp, name, false );
      }
  }




//Accumulate segments connected to component
void SdGraphSymImp::accumLinked(SdPoint a, SdPoint b, SdSelector *sel)
  {
  //Scan all pins and check if segment ab connected to any pin then select component
  for( int index = 0; index < mPins.count(); index++ )
    if( mPins[index].mCom && mPins[index].mPosition.isOnSegment(a,b) ) {
      sel->insert( this );
      return;
      }
  }



void SdGraphSymImp::saveState(SdUndo *undo)
  {
  undo->symImp( &mOrigin, &mProp, &mLogSection, &mLogNumber, &mOverRect, &mPrefix, &mIdentProp, &mIdentOrigin, &mIdentPos, &mIdentRect );
  }




void SdGraphSymImp::moveComplete(SdPoint grid, SdUndo *undo)
  {
  Q_UNUSED(grid);
  //Mooving is completed, check pin connection
  SdPItemSheet *sheet = getSheet();
  QString netName;
  if( sheet ) {
    for( int index = 0; index < mPins.count(); index++ )
      if( !mPins[index].mCom ) {
        //If pin unconnected then check possibilities connection
        if( sheet->getNetFromPoint( mPins[index].mPosition, netName ) ) {
          //New connection
          undo->pinImpConnect( this, index, mPartImp, mPins[index].mPrtPin, mPins[index].mWireName, mPins[index].mCom );
          mPins[index].setConnection( mPartImp, netName, true );
          }
        }

    //Check current area
    if( mPartImp ) {
      //Part is assigned, check current area
      if( sheet->getPlate( mOrigin ) != mPartImp->getPlate() ) {
        //Plate is changed. Unlink from part
        unLinkPartImp( undo );
        }
      else return;
      }
    //Assign part
    linkAutoPart( undo );
    }
  }




void SdGraphSymImp::prepareMove()
  {
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




void SdGraphSymImp::rotate(SdPoint center, SdAngle angle)
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



void SdGraphSymImp::setProp(SdProp &prop)
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



void SdGraphSymImp::getProp(SdProp &prop)
  {
  prop.mSymImpProp.append( mProp );
  }



void SdGraphSymImp::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( !getSelector() && mOverRect.isPointInside(p) )
    selector->insert( this );
  }




void SdGraphSymImp::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( !getSelector() && mOverRect.isAccross(r) )
    selector->insert( this );
  }




void SdGraphSymImp::select(SdSelector *selector)
  {
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
  SdConverterImplement imp( mOrigin, mSymbol->mOrigin, mProp.mAngle.getValue(), mProp.mMirror.getValue() );
  dc->setConverter( &imp );

  //Draw symbol except ident and pins
  mSymbol->forEach( dctAll & ~(dctSymPin | dctIdent), [dc] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph )
      graph->draw( dc );
    return true;
    });

  //Draw pins
  for( int i = 0; i < mPins.count(); i++ )
    mPins[i].draw( dc );
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
      if( !pin.mCom &&
          calcDirection90( snap->mSour, pin.mPosition ) == snap->mDir &&
          snap->test( pin.mPosition, snapNextPin ) ) res = true;
    }
  if( snap->match(snapNearestPin|snapNearestNet) ) {
    for( SdSymImpPin &pin : mPins )
      if( !pin.mCom && snap->test( pin.mPosition, snapNearestPin ) ) res = true;
    }
  return res;
  }




SdPItemSheet *SdGraphSymImp::getSheet() const
  {
  return dynamic_cast<SdPItemSheet*>( getParent() );
  }




void SdGraphSymImp::updatePinsPositions()
  {
  SdConverterImplement impl( mOrigin, mSymbol->mOrigin, mProp.mAngle.getValue(), mProp.mMirror.getValue() );
  QTransform t = impl.getMatrix();
  for( SdSymImpPin &pin : mPins )
    pin.mPosition = t.map( pin.mPin->getPinOrigin() );
  mOverRect.set( t.mapRect( mSymbol->getOverRect() ) );
  mIdentPos = t.map( mIdentOrigin );
  //mIdentRect.set( )
  }




//Unconnect all pins from wires
void SdGraphSymImp::ucomAllPins(SdUndo *undo)
  {
  for( int index = 0; index < mPins.count(); index++ ) {
    //Save state of pins
    undo->pinImpConnect( this, index, mPartImp, mPins[index].mPrtPin, mPins[index].mWireName, mPins[index].mCom );
    mPins[index].mCom = false;
    }
  }




void SdGraphSymImp::createPins(SdUndo *undo)
  {
  //Unconnect all pins from wires
  ucomAllPins( undo );
  //Remove previous pins
  mPins.clear();
  SdSection *s = nullptr;
  //Accumulate pins
  if( mSymbol ) {
    mSymbol->forEach( dctSymPin, [this, s] (SdObject *obj) -> bool {
      SdGraphSymPin *pin = dynamic_cast<SdGraphSymPin*>(obj);
      Q_ASSERT( pin != nullptr );

      //Create implement pin
      SdSymImpPin impPin;
      impPin.mPin     = pin;
      impPin.mPinName = pin->getPinName();

      //Add pin to table
      mPins.append( impPin );
      return true;
      });
    }
  }




//Link auto partImp in given plate. partImp and section are selected automatic
void SdGraphSymImp::linkAutoPartInPlate(SdPItemPlate *plate, SdUndo *undo)
  {

  //Get part where this section resides
  int section;
  SdGraphPartImp *partImp = plate->allocPartImp( &section, mPart, mComponent, mSymbol, undo );
  Q_ASSERT( partImp != nullptr );

  //Link to part
  partImp->setLinkSection( section, this );
  setLinkSection( section, partImp );

  }





void SdGraphSymImp::attach(SdUndo *undo)
  {
  SdProject *prj = getSheet()->getProject();
  Q_ASSERT( prj != nullptr );
  //Realloc objects for this project
  mComponent = dynamic_cast<SdPItemSymbol*>( prj->getProjectsItem(mComponent) );  //Object contains section information, pin assotiation info. May be same as mSymbol.
  mSymbol = dynamic_cast<SdPItemSymbol*>( prj->getProjectsItem(mSymbol) );        //Symbol contains graph information
  mPart = dynamic_cast<SdPItemPart*>( prj->getProjectsItem(mPart) );

  qDebug() << "attach";
  createPins( undo );

  linkAutoPart( undo );
  }




void SdGraphSymImp::detach(SdUndo *undo)
  {
  //Ucon all pins
  ucomAllPins( undo );
  //Unlink from part
  unLinkPartImp( undo );
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
  //mPins         = imp->mPins;        //Pin information table
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
  for( const SdSymImpPin &pin : mPins )
    pins.append( pin.toJson() );
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
    pin.fromJson( map, vpin.toObject() );
    mPins.append( pin );
    }
  //Parameters
  sdParamRead( QStringLiteral("Param"), mParam, obj );
  }
