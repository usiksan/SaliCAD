/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModeTBrowseSheetPart.h"
#include "objects/SdPulsar.h"

SdModeTBrowseSheetPart::SdModeTBrowseSheetPart(SdWEditorGraph *editor, SdProjectItem *obj, SdProjectItem *plate ) :
  SdModeTemp( editor, obj ),
  mSymImp(nullptr),
  mPlate(plate)
  {

  }


SdPoint SdModeTBrowseSheetPart::enterPrev()
  {
  }

QString SdModeTBrowseSheetPart::getStepHelp() const
  {
  }

QString SdModeTBrowseSheetPart::getModeThema() const
  {
  }

QString SdModeTBrowseSheetPart::getStepThema() const
  {
  }

int SdModeTBrowseSheetPart::getCursor() const
  {
  }

int SdModeTBrowseSheetPart::getIndex() const
  {
  }
