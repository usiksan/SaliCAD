#include "SdDMasterSheetDecorator.h"

SdDMasterSheetDecorator::SdDMasterSheetDecorator( SdProjectItem *sheet, QWidget *parent ) :
  QDialog(parent),
  mSheet( dynamic_cast<SdPItemSheet*>(sheet) )
  {

  }
