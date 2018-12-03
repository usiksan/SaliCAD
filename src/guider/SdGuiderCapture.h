/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDGUIDERCAPTURE_H
#define SDGUIDERCAPTURE_H

#include <QObject>
#include <QTimer>

class SdGuiderFile;

class SdGuiderCapture : public QObject
  {
    Q_OBJECT

    QTimer        mTimer;
    SdGuiderFile *mFile;
    QWidget      *mMainWindow;
    int           mTime;
  public:
    explicit SdGuiderCapture(QWidget *main, QObject *parent = nullptr);

    bool isCapture() const { return mFile != nullptr; }

    bool isPaused() const { return !mTimer.isActive(); }

  signals:

  public slots:
    //Init capture process
    void captureInit();

    //
    void captureStop();

    //Pause capture process
    void capturePause();

    //Resume capture process after pause
    void captureResume();
  };

#endif // SDGUIDERCAPTURE_H
