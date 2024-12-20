/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph containet object value
*/
#include "SdGraphValue.h"

SdGraphValue::SdGraphValue()
  {

  }




SdGraphValue::SdGraphValue(SdPoint org, SdRect r, SdPropText &p) :
  SdGraphText( org, "{value}", r, p )
  {

  }




void SdGraphValue::updateValue(SdPoint org, SdRect r, SdPropText *p)
  {
  mOrigin   = org;
  mProp     = *p;
  mString   = QStringLiteral("{value}");
  mOverRect = r;
  }





void SdGraphValue::setText(int index, QString sour, SdPropText &prop, QWidget *parent)
  {
  Q_UNUSED(index);
  Q_UNUSED(sour);
  Q_UNUSED(parent);
  if( sour.isEmpty() )
    mString   = QStringLiteral("{value}");
  else
    mString   = sour;
  mProp   = prop;
  }

