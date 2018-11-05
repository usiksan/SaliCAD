/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for display guider playback
*/
#ifndef SDDGUIDERPLAYER_H
#define SDDGUIDERPLAYER_H

#include "guider/SdGuiderFile.h"

#include <QDialog>
#include <QLabel>
#include <QTimer>

class SdDGuiderPlayer : public QDialog
  {
    Q_OBJECT

    SdGuiderFile  mFile;
    QTimer        mTimer;
    int           mCurrentTime;
    int           mCurrentFrame;
    QLabel       *mView;
    QLabel       *mTiter;
    QString       mFileName;
    QString       mLanguage;
//    bool          mLock;
//    SdGuiderTiter mTiter;
  public:
    SdDGuiderPlayer( const QString fname, QWidget *parent = nullptr );

    static QString guiderPath();

    static bool    guiderExist( const QString fname );
  signals:
    void sgPlay( bool start );

  public slots:
    void cmPlayRestart();
    void cmPlayStart();
    void cmPlayPause();
    void cmPlayStop();

    void play();

  private:
    void titerChanged();
  };

#endif // SDDGUIDERPLAYER_H
