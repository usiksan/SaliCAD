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

namespace Ui {
class SdDRegistation;
}

class SdDRegistation : public QDialog
  {
    Q_OBJECT
    bool              mFromHelp;
    int               mNameStatus; //0-empty, 1-sali, 2-other
  public:
    explicit SdDRegistation( bool fromHelp = true, QWidget *parent = nullptr);
    ~SdDRegistation();

  public slots:
    //Registration new user
    void cmRegistration();


    void cmGeneratePrivateKey();

    void cmGetStatus();

    //On edit user name
    void onEditAuthorName( const QString nm );

    //Close dialog
    void cmClose();

  private:
    Ui::SdDRegistation *ui;

    static void performRegistration( SdDRegistation *reg );

    static void performGetStatus( SdDRegistation *reg );
  };

#endif // SDDREGISTATION_H
