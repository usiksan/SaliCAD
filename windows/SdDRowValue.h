/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDROWVALUE_H
#define SDDROWVALUE_H

#include <QDialog>

namespace Ui {
  class SdDRowValue;
}

class SdDRowValue : public QDialog
  {
    Q_OBJECT

  public:
    explicit SdDRowValue(QWidget *parent = nullptr);
    ~SdDRowValue();

  private:
    Ui::SdDRowValue *ui;
  };

#endif // SDDROWVALUE_H
