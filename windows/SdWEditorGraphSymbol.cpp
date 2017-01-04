/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdWEditorGraphSymbol.h"

SdWEditorGraphSymbol::SdWEditorGraphSymbol(SdSymbol *sym, QWidget *parent) :
  SdWEditorGraph( parent ),
  mSymbol(sym)
  {

  }



SdProjectItem *SdWEditorGraphSymbol::getProjectItem()
  {
  return mSymbol;
  }
