/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for bus enter
*/
#ifndef SDDGETBUS_H
#define SDDGETBUS_H

#include <QDialog>

namespace Ui {
class SdDGetBus;
}

class SdDGetBus : public QDialog
  {
    Q_OBJECT

  public:
    explicit SdDGetBus(QWidget *parent = 0);
    ~SdDGetBus();

    QStringList busList() const;

  private:
    Ui::SdDGetBus *ui;

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDGETBUS_H
