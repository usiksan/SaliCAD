/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for default param selection and insertion.
  All parameters listed int table with description.
  User can check one or more parameters for insertion.
*/
#ifndef SDDPARAMDEFAULT_H
#define SDDPARAMDEFAULT_H

#include <QDialog>

namespace Ui {
  class SdDParamDefault;
  }

class SdDParamDefault : public QDialog
  {
    Q_OBJECT

  public:
    explicit SdDParamDefault(QWidget *parent = nullptr);
    ~SdDParamDefault();

    //Return list of default params for insertion
    QStringList defParamList() const;

  protected:
    void changeEvent(QEvent *e);

  private:
    Ui::SdDParamDefault *ui;
  };

#endif // SDDPARAMDEFAULT_H
