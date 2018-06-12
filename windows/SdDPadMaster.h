/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDPADMASTER_H
#define SDDPADMASTER_H

#include <QDialog>

namespace Ui {
  class SdDPadMaster;
}

class SdDPadMaster : public QDialog
  {
    Q_OBJECT

  public:
    explicit SdDPadMaster(QWidget *parent = 0);
    ~SdDPadMaster();

  private:
    Ui::SdDPadMaster *ui;
  };

#endif // SDDPADMASTER_H
