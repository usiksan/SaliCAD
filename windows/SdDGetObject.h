/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDDGETOBJECT_H
#define SDDGETOBJECT_H

#include <QDialog>

namespace Ui {
class SdDGetObject;
}

class SdDGetObject : public QDialog
  {
    Q_OBJECT

  public:
    explicit SdDGetObject(QWidget *parent = 0);
    ~SdDGetObject();

  protected:
    void changeEvent(QEvent *e);

  private:
    Ui::SdDGetObject *ui;
  };

#endif // SDDGETOBJECT_H
