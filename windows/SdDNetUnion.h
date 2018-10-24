/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for selection result name when union two nets.
*/
#ifndef SDDNETUNION_H
#define SDDNETUNION_H

#include <QDialog>

namespace Ui {
  class SdDNetUnion;
}

class SdDNetUnion : public QDialog
  {
    Q_OBJECT

  public:
    explicit SdDNetUnion(const QString first, const QString second, QWidget *parent = nullptr);
    ~SdDNetUnion() override;

  private:
    Ui::SdDNetUnion *ui;

    // QDialog interface
  public slots:
    virtual void accept() override;
    virtual void reject() override;
  };

#endif // SDDNETUNION_H
