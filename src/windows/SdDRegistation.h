/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Registration dialog.

  Registration is to set the name of user with appropriate key. By this name and key
  server counts object transfered to user.
*/
#ifndef SDDREGISTATION_H
#define SDDREGISTATION_H

#include <QDialog>
//#include <QTimer>

namespace Ui {
class SdDRegistation;
}

class SdDRegistation : public QDialog
  {
    Q_OBJECT

    //QTimer mPeriodic;   //Timer for periodic check registration status
    bool   mFromHelp;
    int    mNameStatus; //0-empty, 1-sali, 2-other
  public:
    explicit SdDRegistation( bool fromHelp = true, QWidget *parent = nullptr);
    ~SdDRegistation();

  signals:
    //Signal to start registration
    void doRegistration( const QString ip, const QString authorName, const QString email );

    //Signal to add machine
    void doMachine( const QString ip, const QString authorName, quint64 key );

  public slots:
    //Registration new user
    void cmRegistration();

    //Add to user new machine
    void cmAddMachine();

    //On complete registration
    void onRegistrationComplete(const QString authorName, const QString email, quint64 key, int remain, int result);

    //On edit user name
    void onEditAuthorName( const QString nm );

    //Close dialog
    void cmClose();

  private:
    Ui::SdDRegistation *ui;
  };

#endif // SDDREGISTATION_H
