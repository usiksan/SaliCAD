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
#include "SdPItemPlate.h"
#include "SdPItemSheet.h"
#include "SdPItemPart.h"
#include "SdProject.h"
#include "SdSection.h"
#include "SdSelector.h"
#include "SdConverterImplement.h"
#include "SdContext.h"
#include "SdUndo.h"


//====================================================================================
//Pin for symbol implementation

SdSymImpPin::SdSymImpPin() :
  mPin(0),       //Pin
  mCom(false),   //State of pin to net connectivity
  mPrtPin(-1)    //Pin index in pin array of part implement
  {

  }



void SdSymImpPin::operator =(SdSymImpPin &pin)
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
  return !mCom && mPosition.isOnSection( a, b );
  }




bool SdSymImpPin::isCanDisconnect(SdPoint a, SdPoint b, const QString wireName) const
  {
  return mCom && mWireName == wireName && mPosition.isOnSection( a, b );
  }






void SdSymImpPin::ucom(SdGraphPartImp *prt)
  {
  if( mCom ) {
    mCom = false;
    if( prt ) prt->pinConnectionSet( mPinNumber, mWireName, mCom );
    }
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
    SdConverterImplement imp( nullptr, mOrigin, sym->mOrigin, mProp.mAngle.getValue(), mProp.mMirror.getValue() );
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




//Get full visual ident of section aka D4.2
QString SdGraphSymImp::getIdent() const
  {
  if( mLogSection )
    return QString("%1%2.%3").arg( mPrefix ).arg( mLogNumber ).arg( mLogSection );
  return QString("%1%2").arg( mPrefix ).arg( mLogNumber );
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
    if( mPins[index].mCom && mPins[index].mPosition.isOnSection(a,b) ) {
      sel->insert( this );
      return;
      }
  }



void SdGraphSymImp::saveState(SdUndo *undo)
    {
    }




void SdGraphSymImp::moveComplete(SdUndo *undo)
  {
  //Mooving is completed, check pin connection
  SdPItemSheet *sheet = getSheet();
  QString netName;
  if( sheet ) {
    for( int index = 0; index < mPins.count(); index++ )
      if( !mPins[index].mCom ) {
        //If pin unconnected then check possible connection
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
        //Плата изменилась
        UnLinkPrt();
        }
      else return;
      }
    //Нужно назначить корпус автоматически
    ConnectToComp( 0 );
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
    ucomAllPins();
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
  SdConverterImplement imp( dc, mOrigin, mSymbol->mOrigin, mProp.mAngle.getValue(), mProp.mMirror.getValue() );
  dc->setConverter( &imp );
  //Рисовать выводы
  DrawSymImpPinIterator draw( dc );
  NForEach( pins, draw );
  //Рисовать корпус
  DImplementConverter conv( dc, symbol->GetOrigin(), info.origin, info.prop.angle, info.prop.mirror, 0 );
  symbol->ForEachPic( DrawGraphIterator(conv) );
  symbol->ForEachPin( DrawSymPinIterator(conv,info.section) );
  }

int SdGraphSymImp::behindCursor(SdPoint p)
  {
  }

int SdGraphSymImp::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
    {
    }

bool SdGraphSymImp::getInfo(SdPoint p, QString &info, bool extInfo)
    {
    }

bool SdGraphSymImp::snapPoint(SdSnapInfo *snap)
  {
  }




SdPItemSheet *SdGraphSymImp::getSheet() const
  {
  return dynamic_cast<SdPItemSheet*>( getParent() );
  }




void SdGraphSymImp::updatePinsPositions()
  {
  SdConverterImplement impl( nullptr, mOrigin, mSymbol->mOrigin, mProp.mAngle.getValue(), mProp.mMirror.getValue() );
  QTransform t = impl.getMatrix();
  for( SdSymImpPin &pin : mPins )
    pin.mPosition = t.map( pin.mPin->getPinOrigin() );
  mOverRect.set( t.mapRect( mSymbol->getOverRect() ) );
  mIdentPos = t.map( mIdentOrigin );
  //mIdentRect.set( )
  }




//Unconnect all pins from wires
void SdGraphSymImp::ucomAllPins()
  {
  for( SdSymImpPin &pin : mPins )
    pin.ucom( mPartImp );
  }



void SdGraphSymImp::createPins()
  {
  //Unconnect all pins from wires
  ucomAllPins();
  //Remove previous pins
  mPins.clear();
  SdSection *s = nullptr;
  if( mComponent )
    s = mComponent->getSection( mSectionIndex );
  //Accumulate pins
  if( mSymbol ) {
    mSymbol->forEach( dctSymPin, [this, s] (SdObject *obj) -> bool {
      SdGraphSymPin *pin = dynamic_cast<SdGraphSymPin*>(obj);
      if( pin ) {
        SdSymImpPin impPin;
        impPin.mPin     = pin;
        impPin.mPinName = pin->getPinName();
        if( s )
          impPin.mPinNumber = s->getPinNumber( impPin.mPinName );
        }
      return true;
      });
    }
  }




void SdGraphSymImp::unLinkFromPart()
  {
  //Отвязать выводы
  UnLinkIterator iterator( prtImp );
  NForEach( pins, iterator );
  //Отключение от компонента
  if( prtImp ) prtImp->Remove( this );
  prtImp = 0;
  }





void SdGraphSymImp::attach(SdUndo *undo)
  {
  SdProject *prj = getSheet()->getProject();
  Q_ASSERT( prj != nullptr );
  //Realloc objects for this project
  mComponent = dynamic_cast<SdPItemSymbol*>( prj->getProjectsItem(mComponent) );  //Object contains section information, pin assotiation info. May be same as mSymbol.
  mSymbol = dynamic_cast<SdPItemSymbol*>( prj->getProjectsItem(mSymbol) );        //Symbol contains graph information
  mPart = dynamic_cast<SdPItemPart*>( prj->getProjectsItem(mPart) );

  //Get plate where component resides
  SdPItemPlate *plate = getSheet()->getPlate( mOrigin );
  Q_ASSERT( plate != nullptr );

  //Get part where this section resides
  int section;
  SdGraphPartImp *partImp = plate->allocPartImp( &section, mPart, mComponent, this );

  //Assign pins
  }

void SdGraphSymImp::detach(SdUndo *undo)
  {
  }

void SdGraphSymImp::cloneFrom(const SdObject *src)
  {
  }

void SdGraphSymImp::writeObject(QJsonObject &obj) const
  {
  }

void SdGraphSymImp::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  }
