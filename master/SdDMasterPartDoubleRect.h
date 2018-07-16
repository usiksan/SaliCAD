/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDMASTERPARTDOUBLERECT_H
#define SDDMASTERPARTDOUBLERECT_H

#include "SdDMasterPart.h"

namespace Ui {
  class SdDMasterPartDoubleRect;
}

class SdDMasterPartDoubleRect : public SdDMasterPart
  {
    Q_OBJECT

  public:
    explicit SdDMasterPartDoubleRect( SdProjectItem *item, QWidget *parent = nullptr);
    ~SdDMasterPartDoubleRect();

  private:
    Ui::SdDMasterPartDoubleRect *ui;
  };

#endif // SDDMASTERPARTDOUBLERECT_H
