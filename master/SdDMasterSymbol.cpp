/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base class for symbol masters.

  Contains operations for append graphics and pins to symbol when creation.
*/
#include "SdDMasterSymbol.h"
#include "objects/SdGraphLinearLine.h"
#include "objects/SdGraphLinearRect.h"
#include "objects/SdGraphLinearCircle.h"
#include "objects/SdGraphSymPin.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphIdent.h"
#include "objects/SdGraphValue.h"


void SdDMasterSymbol::addLine(int x1, int y1, int x2, int y2)
  {
  mItem->insertChild( new SdGraphLinearLine( SdPoint(x1, y1), SdPoint(x2,y2), mLineProp), nullptr );
  }




void SdDMasterSymbol::addRect(int x1, int y1, int x2, int y2)
  {
  mItem->insertChild( new SdGraphLinearRect( SdPoint(x1, y1), SdPoint(x2,y2), mLineProp), nullptr );
  }




void SdDMasterSymbol::addCircle(int cx, int cy, int r)
  {
  mItem->insertChild( new SdGraphLinearCircle( SdPoint(cx,cy), r, mLineProp), nullptr );
  }



//Identifier append to "id" layer
void SdDMasterSymbol::setId(SdPoint p, int size)
  {
  SdGraphIdent *ident = mItem->identGet();
  int half = size / 2;
  SdRect r( p.x()-half, p.y()-half, size, size );
  mIdentProp.mSize = size;
  ident->updateIdent( p, r, &mIdentProp );
  }




//Value append to "value" layer
void SdDMasterSymbol::setValue(SdPoint p, int size)
  {
  SdGraphValue *value = mItem->valueGet();
  int half = size / 2;
  SdRect r( p.x()-half, p.y()-half, size, size );
  mValueProp.mSize = size;
  value->updateValue( p, r, &mValueProp );
  }







//Pin append to "pin" layer
void SdDMasterSymbol::addPin(SdPoint org, int type, SdPoint pinNameOrg, const QString pinName, SdPoint pinNumberOrg)
  {
  mPinProp.mPinType = type;
  mItem->insertChild( new SdGraphSymPin( org, mPinProp, pinNumberOrg, mPinNumberProp, pinNameOrg, mPinNameProp, pinName), nullptr );
  }










SdDMasterSymbol::SdDMasterSymbol(SdProjectItem *item, QWidget *parent) :
  QDialog( parent ),
  mItem(item)
  {
  mLineProp.mLayer.set( LID0_COMPONENT );
  mLineProp.mType  = dltSolid;
  mLineProp.mWidth = 0;

  mPinProp.mLayer.set( LID0_PIN );
  mPinProp.mPinType = 0;

  mPinNumberProp.mLayer.set( LID0_PIN_NUMBER );
  mPinNumberProp.mFont   = 0;
  mPinNumberProp.mSize   = 350;
  mPinNumberProp.mDir    = da0;
  mPinNumberProp.mHorz   = dhjLeft;
  mPinNumberProp.mVert   = dvjMiddle;
  mPinNumberProp.mMirror = 0;

  mPinNameProp.mLayer.set( QString(LID0_PIN_NAME) );    //Свойства имени вывода
  mPinNameProp.mFont   = 0;
  mPinNameProp.mSize   = 350;
  mPinNameProp.mDir    = da0;
  mPinNameProp.mHorz   = dhjLeft;
  mPinNameProp.mVert   = dvjMiddle;
  mPinNameProp.mMirror = 0;

  mIdentProp.mLayer.set( QString(LID0_IDENT) );    //Layer of ident
  mIdentProp.mFont   = 0;
  mIdentProp.mSize   = 350;
  mIdentProp.mDir    = da0;
  mIdentProp.mHorz   = dhjCenter;
  mIdentProp.mVert   = dvjMiddle;
  mIdentProp.mMirror = 0;

  mValueProp.mLayer.set( QString(LID0_VALUE) );    //Layer of value
  mValueProp.mFont   = 0;
  mValueProp.mSize   = 350;
  mValueProp.mDir    = da0;
  mValueProp.mHorz   = dhjCenter;
  mValueProp.mVert   = dvjMiddle;
  mValueProp.mMirror = 0;

  }

