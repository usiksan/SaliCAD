/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDNETCLIENT_H
#define SDDNETCLIENT_H

#include <QDialog>

namespace Ui {
class SdDNetClient;
}

class SdDNetClient : public QDialog
  {
    Q_OBJECT

    QString mId;
  public:
    explicit SdDNetClient( const QString id, QWidget *parent = 0);
    ~SdDNetClient();

    //Get object from remote server
    static bool getObject( QWidget *parent, const QString id );

    //Register new user
    static bool registerAuthor( QWidget *parent, const QString author, const QString descr, int limit );

  signals:
    void requestObject( const QString id );

  public slots:
    void onCompleteRequestObject( bool res );

    void onProcess( int pos, int range, const QString oper );

  private:
    Ui::SdDNetClient *ui;
  };

#endif // SDDNETCLIENT_H
