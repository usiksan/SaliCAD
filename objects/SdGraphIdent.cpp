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



SdGraphIdent::SdGraphIdent(SdPoint org, const QString str, SdRect r, SdPropText &p) :
  SdGraphText( org, str, r, p )
  {
  }



void SdGraphIdent::updateIdent(SdPoint org, const QString str, SdRect r, SdPropText *p)
  {
  mOrigin   = org;
  mProp     = *p;
  mString   = str;
  mOverRect = r;
  }


