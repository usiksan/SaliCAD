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
  mLayer(0),     //Pin layer
  mCom(false)    //State of pin to net connectivity
  {

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



void SdGraphSymImp::saveState(SdUndo *undo)
    {
    }




void SdGraphSymImp::moveComplete(SdUndo *undo)
  {
  //Перенос завершен - проверить подключения выводов
  if( GetSheet() ) {
    ConnectIterator connect( *GetSheet(), prtImp );
    NForEach( pins, connect );
    //Проверить область
    if( prtImp ) {
      //Корпус назначен, проверить местонахождение
      if( GetSheet()->GetPlate( info.over.LeftBottom() ) != prtImp->GetPlate() ) {
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
      pin.prepareMove( sel, sheet );
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
    pin.mPosition = t.map( pin.mOrigin );
  mOverRect = t.mapRect( mSymbol->getOverRect() );
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
        impPin.mLayer = pin->getPinLayer();
        impPin.mOrigin = pin->getPinOrigin();
        impPin.mPinName = pin->getPinName();
        if( s )
          impPin.mPinNumber = s->getPinNumber( impPin.mPinName );
        }
      return true;
      });
    }
  }



