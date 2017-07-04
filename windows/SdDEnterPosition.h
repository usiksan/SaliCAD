/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDENTERPOSITION_H
#define SDDENTERPOSITION_H

#include <QDialog>

namespace Ui {
  class SdDEnterPosition;
  }

class SdDEnterPosition : public QDialog
  {
    Q_OBJECT

  public:
    explicit SdDEnterPosition(QWidget *parent = 0);
    ~SdDEnterPosition();

    bool getRef() const;
    int  getX() const;
    int  getY() const;
    int  getZ() const;

  protected:
    void changeEvent(QEvent *e);

  private:
    Ui::SdDEnterPosition *ui;
  };

#endif // SDDENTERPOSITION_H
