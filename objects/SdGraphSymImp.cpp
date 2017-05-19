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
#include "SdPItemPlate.h"
#include "SdPItemSheet.h"
#include "SdSection.h"
#include "SdSelector.h"
#include "SdConverterImplement.h"


//====================================================================================
//Pin for symbol implementation

SdSymImpPin::SdSymImpPin() :
  mPin(0),       //Pin
  mCom(false),   //State of pin to net connectivity
  mPinIndex(-1), //Pin index in pin array
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
  mPinIndex  = pin.mPinIndex;  //Pin index in pin array
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
SdGraphSymImp::SdGraphSymImp()
  {

  }




//Pin connection-disconnection by index
void SdGraphSymImp::pinConnectionSet(int pinIndex, const QString wireName, bool com)
  {
  Q_ASSERT( pinIndex >= 0 && pinIndex < mPins.count() );
  mPins[pinIndex].setConnection( wireName, com );
  }

QString SdGraphSymImp::pinWireName(int pinIndex) const
  {
  Q_ASSERT( pinIndex >= 0 && pinIndex < mPins.count() );
  return mPins[pinIndex].mWireName;
  }

bool SdGraphSymImp::isPinConnected(int pinIndex) const
  {
  Q_ASSERT( pinIndex >= 0 && pinIndex < mPins.count() );
  return mPins[pinIndex].mCom;
  }



void SdGraphSymImp::netWirePlace(SdPoint a, SdPoint b, const QString name, SdUndo *undo)
  {
  for( int index = 0; index < mPins.count(); index++ )
    if( mPins[index].isCanConnect( a, b ) ) {
      //Undo previous state of pin
      undo->pinImpConnect( this, index, mPartImp, mPins[index].mPrtPin, mPins[index].mWireName, mPins[index].mCom );
      //Set new state of pin
      mPins[index].setConnection( name, true );
      if( mPartImp )
        mPartImp->pinConnectionSet( mPins[index].mPrtPin, name, true );
      }
  }




void SdGraphSymImp::netWireDelete(SdPoint a, SdPoint b, const QString name, SdUndo *undo)
  {
  for( int index = 0; index < mPins.count(); index++ )
    if( mPins[index].isCanDisconnect( a, b, name ) ) {
      //Undo previous state of pin
      undo->pinImpConnect( this, index, mPartImp, mPins[index].mPrtPin, mPins[index].mWireName, mPins[index].mCom );
      //Set new state of pin
      mPins[index].setConnection( name, false );
      if( mPartImp )
        mPartImp->pinConnectionSet( mPins[index].mPrtPin, name, false );
      }
  }




void SdGraphSymImp::accumLinked(SdPoint a, SdPoint b, SdSelector *sel)
  {
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
  if( sheet ) {
    for( SdSymImpPin &pin : mPins )
      pin.connectTest( sheet, mPartImp );
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
  //Draw ident
  DString str;
  GetIdent( str );
  dc.Text( info.ident, str );
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
