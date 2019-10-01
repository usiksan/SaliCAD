/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Sheet decorator master.

  Appends sheet decoration to new created sheet
*/
#ifndef SDDMASTERSHEETDECORATOR_H
#define SDDMASTERSHEETDECORATOR_H

#include "objects/SdPItemSheet.h"

#include <QDialog>

class SdDMasterSheetDecorator : public QDialog
  {
    Q_OBJECT

    SdPItemSheet *mSheet;
  public:
    SdDMasterSheetDecorator( SdProjectItem *sheet, QWidget *parent );
  };

#endif // SDDMASTERSHEETDECORATOR_H
