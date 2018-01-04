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

  signals:
    void doRegistration( const QString ip, const QString authorName, const QString description );

    void doMachine( const QString ip, const QString authorName, const QString key );

  public slots:
    //Registration new user
    void cmRegistration();

    //Add to user new machine
    void cmAddMachine();

    //On complete registration
    void onRegistrationComplete( const QString authorName, const QString descr, const QString key, int limit, int delivered, int result );

    //Close dialog
    void cmClose();

  private:
    Ui::SdDRegistation *ui;
  };

#endif // SDDREGISTATION_H
