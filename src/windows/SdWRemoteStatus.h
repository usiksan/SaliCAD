/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Remote library interchange status
*/
#ifndef SDWREMOTESTATUS_H
#define SDWREMOTESTATUS_H


#include <QWidget>
#include <QListWidget>
#include <QTimer>

class SdWRemoteStatus : public QListWidget
  {
    Q_OBJECT

    QTimer mHideTimer; //Timer for automatic hide widget
  public:
    explicit SdWRemoteStatus(QWidget *parent = nullptr);
    ~SdWRemoteStatus();

    static SdWRemoteStatus *mWRemoteStatus;
  signals:

  public slots:
    void addInfo( const QString info );

  protected slots:
    void restartTimer();

    // QWidget interface
  protected:
    virtual void closeEvent(QCloseEvent *event) override;
  };

#endif // SDWREMOTESTATUS_H
