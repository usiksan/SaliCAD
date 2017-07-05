/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for handle enter position of cursor
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

    bool     getRef() const;
    QString  getX() const;
    QString  getY() const;
    QString  getZ() const;

  protected:
    void changeEvent(QEvent *e);

  private:
    Ui::SdDEnterPosition *ui;

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDENTERPOSITION_H
