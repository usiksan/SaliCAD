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
#include <QTableWidget>

class SdGuiderWMain : public QMainWindow
  {
    Q_OBJECT

    SdGuiderFile  mFile;
    QTimer        mTimer;
    int           mCurrentTime;
    QLabel       *mView;
    QListWidget  *mTimeList;
    QString       mFileName;
    bool          mLock;
    QTableWidget *mTiterTable;
    QString       mLanguage;
  public:
    explicit SdGuiderWMain(QWidget *parent = nullptr);

  signals:

  public slots:
    void cmFileOpen();
    void cmFileSave();

    void cmTiterEdit();
    void cmTiterInsert();
    void cmTiterHide();
    void cmTiterCopy();
    void cmTiterCut();
    void cmTiterPaste();
    void cmTiterLanguage();

    void cmPlayRestart();
    void cmPlayStart();
    void cmPlayStop();

    void play();

    void onRowChanged( int row );

    void onCellChanged( int row, int column );

  private:
    void refreshTime();

    void titerChanged();
  };

#endif // SDGUIDERWMAIN_H
