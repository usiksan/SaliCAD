/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base class for part masters.

  Contains operations for append graphics and pins to part when creation.
*/
#include "SdDMasterPart.h"
#include "objects/SdGraphLinearLine.h"
#include "objects/SdGraphLinearRect.h"
#include "objects/SdGraphLinearCircle.h"
#include "objects/SdGraphPartPin.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphIdent.h"
#include "objects/SdGraphValue.h"


void SdDMasterPart::addLine(int x1, int y1, int x2, int y2)
  {
  mItem->insertChild( new SdGraphLinearLine( SdPoint(x1, y1), SdPoint(x2,y2), mLineProp), nullptr );
  }




void SdDMasterPart::addRect(int x1, int y1, int x2, int y2)
  {
  mItem->insertChild( new SdGraphLinearRect( SdPoint(x1, y1), SdPoint(x2,y2), mLineProp), nullptr );
  }




void SdDMasterPart::addCircle(int cx, int cy, int r)
  {
  mItem->insertChild( new SdGraphLinearCircle( SdPoint(cx,cy), r, mLineProp), nullptr );
  }



//Identifier append to "id" layer
void SdDMasterPart::setId(SdPoint p, int size)
  {
  SdGraphIdent *ident = mItem->identGet();
  int half = size / 2;
  SdRect r( p.x()-half, p.y()-half, size, size );
  mIdentProp.mSize = size;
  ident->updateIdent( p, r, &mIdentProp );
  }




//Value placeholder append to "value" layer
void SdDMasterPart::setValue(SdPoint p, int size)
  {
  SdGraphValue *value = mItem->valueGet();
  int half = size / 2;
  SdRect r( p.x()-half, p.y()-half, size, size );
  mValueProp.mSize = size;
  value->updateValue( p, r, &mValueProp );
  }





void SdDMasterPart::setupSmdPin()
  {
  mPinProp.mLayer.set( LID0_PIN );
  mPinProp.mSide  = stmTop;
  }





void SdDMasterPart::setupThrouPin()
  {
  mPinProp.mLayer.set( LID0_PIN );
  mPinProp.mSide  = stmThrough;
  }




void SdDMasterPart::addPin(SdPoint org, const QString type, SdPoint pinNumberOrg, const QString pinNumber, SdPoint pinNameOrg)
  {
  mPinProp.mPinType = type;
  mItem->insertChild( new SdGraphPartPin( org, mPinProp, pinNumberOrg, mPinNumberProp, pinNameOrg, mPinNameProp, pinNumber), nullptr );
  }



SdDMasterPart::SdDMasterPart(SdProjectItem *item, QWidget *parent) :
  QDialog( parent ),
  mItem(item)
  {
  mLineProp.mLayer.set( LID0_COMPONENT );
  mLineProp.mType  = dltSolid;
  mLineProp.mWidth = 0;

  setupSmdPin();

  mPinNumberProp.mLayer.set( LID0_PIN_NUMBER LID1_TOP );
  mPinNumberProp.mFont   = 0;
  mPinNumberProp.mSize   = 500;
  mPinNumberProp.mDir    = da0;
  mPinNumberProp.mHorz   = dhjLeft;
  mPinNumberProp.mVert   = dvjMiddle;
  mPinNumberProp.mMirror = 0;

  mPinNameProp.mLayer.set( QString(LID0_PIN_NAME LID1_TOP) );    //Свойства имени вывода
  mPinNameProp.mFont   = 0;
  mPinNameProp.mSize   = 500;
  mPinNameProp.mDir    = da0;
  mPinNameProp.mHorz   = dhjLeft;
  mPinNameProp.mVert   = dvjMiddle;
  mPinNameProp.mMirror = 0;

  mIdentProp.mLayer.set( QString(LID0_IDENT LID1_TOP) );    //Layer of ident
  mIdentProp.mFont   = 0;
  mIdentProp.mSize   = 1000;
  mIdentProp.mDir    = da0;
  mIdentProp.mHorz   = dhjCenter;
  mIdentProp.mVert   = dvjMiddle;
  mIdentProp.mMirror = 0;

  mValueProp.mLayer.set( QString(LID0_VALUE LID1_TOP) );    //Layer of value
  mValueProp.mFont   = 0;
  mValueProp.mSize   = 1000;
  mValueProp.mDir    = da0;
  mValueProp.mHorz   = dhjCenter;
  mValueProp.mVert   = dvjMiddle;
  mValueProp.mMirror = 0;

  //Clear item
  mItem->clear();
  }


