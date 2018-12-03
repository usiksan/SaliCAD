/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part master with rectangle body and two pins columns by left and right sides
*/
#ifndef SDDMASTERPARTDOUBLESIDELR_H
#define SDDMASTERPARTDOUBLESIDELR_H

#include "SdDMasterPart.h"
#include "windows/SdIllustrator.h"

namespace Ui {
  class SdDMasterPartDoubleSideLR;
}

class SdDMasterPartDoubleSideLR : public SdDMasterPart
  {
    Q_OBJECT

  public:
    explicit SdDMasterPartDoubleSideLR(SdProjectItem *item, QWidget *parent = nullptr);
    ~SdDMasterPartDoubleSideLR() override;

  public slots:
    //Update preview on any params changed
    void onEditChanged( const QString txt );

  private:
    Ui::SdDMasterPartDoubleSideLR *ui;

    //Draw part preview
    void drawPart( SdIllustrator &il );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDMASTERPARTDOUBLESIDELR_H
