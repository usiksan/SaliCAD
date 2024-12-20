/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph containet object ident
*/
#include "SdGraphIdent.h"

SdGraphIdent::SdGraphIdent() :
  SdGraphText()
  {
  }



SdGraphIdent::SdGraphIdent(SdPoint org, SdRect r, SdPropText &p) :
  SdGraphText( org, "{id}", r, p )
  {
  }



void SdGraphIdent::updateIdent(SdPoint org, SdRect r, SdPropText *p)
  {
  mOrigin   = org;
  mProp     = *p;
  mString   = QStringLiteral("{id}");
  mOverRect = r;
  }




void SdGraphIdent::setText(int index, QString sour, SdPropText &prop, QWidget *parent)
  {
  Q_UNUSED(index);
  Q_UNUSED(sour);
  Q_UNUSED(parent);
  if( sour.isEmpty() )
    mString   = QStringLiteral("{id}");
  else
    mString   = sour;
  mProp   = prop;
  }


