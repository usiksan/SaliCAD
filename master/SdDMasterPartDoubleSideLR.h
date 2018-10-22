/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDMASTERPARTDOUBLESIDELR_H
#define SDDMASTERPARTDOUBLESIDELR_H

#include <QDialog>

namespace Ui {
  class SdDMasterPartDoubleSideLR;
}

class SdDMasterPartDoubleSideLR : public QDialog
  {
    Q_OBJECT

  public:
    explicit SdDMasterPartDoubleSideLR(QWidget *parent = nullptr);
    ~SdDMasterPartDoubleSideLR();

  private:
    Ui::SdDMasterPartDoubleSideLR *ui;
  };

#endif // SDDMASTERPARTDOUBLESIDELR_H
