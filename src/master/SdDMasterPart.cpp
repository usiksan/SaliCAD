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




SdDMasterPart::SdDMasterPart(SdProjectItem *item, QWidget *parent) :
  QDialog( parent ),
  mMasterPart(item)
  {
  //Clear item
  item->clear();
  }


