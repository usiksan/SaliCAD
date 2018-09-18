/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDGUIDERWMAIN_H
#define SDGUIDERWMAIN_H

#include "guider/SdGuiderFile.h"

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QListWidget>

class SdGuiderWMain : public QMainWindow
  {
    Q_OBJECT

    SdGuiderFile mFile;
    QTimer       mTimer;
    int          mCurrentTime;
    //int
    QLabel      *mView;
    QListWidget *mTimeList;
    QString      mFileName;
  public:
    explicit SdGuiderWMain(QWidget *parent = nullptr);

  signals:

  public slots:
    void cmFileOpen();
    void cmFileSave();

    void cmPlayRestart();
    void cmPlayStart();
    void cmPlayStop();

    void play();
  };

#endif // SDGUIDERWMAIN_H
