/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDREGISTATION_H
#define SDDREGISTATION_H

#include <QDialog>

namespace Ui {
class SdDRegistation;
}

class SdDRegistation : public QDialog
  {
    Q_OBJECT

  public:
    explicit SdDRegistation(QWidget *parent = 0);
    ~SdDRegistation();

  private:
    Ui::SdDRegistation *ui;
  };

#endif // SDDREGISTATION_H
