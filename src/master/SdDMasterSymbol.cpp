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
#include "objects/SdGraphText.h"
#include "objects/SdGraphSymPin.h"
#include "objects/SdPropDef.h"
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




void SdDMasterSymbol::addText(int x, int y, const SdPropText &prp, const QString &text)
  {
  SdRect r;
  mItem->insertChild( new SdGraphText( SdPoint(x,y), text, r, prp ), nullptr );
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
  mLineProp = SdPropDef::get().mSymLineProp;

  mPinProp = SdPropDef::get().mSymPinProp;

  mPinNumberProp = SdPropDef::get().mSymPinNumberProp;

  mPinNameProp = SdPropDef::get().mSymPinNameProp;

  mIdentProp = SdPropDef::get().mSymIdentProp;

  mValueProp = SdPropDef::get().mSymValueProp;

  //Clear item
  mItem->clear();
  }

