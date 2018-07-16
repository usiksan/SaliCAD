/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDMASTERPART_H
#define SDDMASTERPART_H

#include "objects/SdProjectItem.h"
#include <QDialog>

class SdDMasterPart : public QDialog
  {
    Q_OBJECT

  protected:
    SdProjectItem *mItem;
  public:
    SdDMasterPart( SdProjectItem *item, QWidget *parent );

  protected:
  };

#endif // SDDMASTERPART_H
