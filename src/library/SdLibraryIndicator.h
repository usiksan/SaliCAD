#ifndef SDLIBRARYINDICATOR_H
#define SDLIBRARYINDICATOR_H

#include "SvLib/SvSingleton.h"

#include <QObject>
#include <QAction>
#include <QLabel>
#include <QStatusBar>

class SdLibraryIndicator : public QObject
  {
    Q_OBJECT

    QAction *mGlobalStorage;
    QAction *mPrivateCloud;
    QAction *mLocalLibrary;
    QLabel  *mGlobalStorageIndicator;
    QLabel  *mPrivateCloudIndicator;
    QLabel  *mLocalLibraryIndicator;

    QString  mGlobalErrorMsg;
    QString  mPrivateErrorMsg;
    QString  mLocalErrorMsg;

    QString  mMsgNone;
    QString  mMsgOk;
    QString  mMsgTransfer;
    QString  mMsgError;

    SdLibraryIndicator();
  public:
    SV_SINGLETON( SdLibraryIndicator )

    enum SdLibraryIndicatorStatus {
      SdLisNoTransfer,
      SdLisTransfer,
      SdLisError
      };

    void addToStatusBar( QStatusBar *sbar );

    // QAction *globalStorage() const { return mGlobalStorage; }
    // QAction *privateCloud() const { return mPrivateCloud; }
    // QAction *localLibrary() const { return mLocalLibrary; }
    // QLabel  *globalStorageIndicator() const { return mGlobalStorageIndicator; }
    // QLabel  *privateCloudIndicator() const { return mPrivateCloudIndicator; }
    // QLabel  *localLibraryIndicator() const { return mLocalLibraryIndicator; }

  public slots:
    void setGlobalStorage( int transferOut, int transferIn, int stat, const QString errorMsg );

    void setPrivateCloud(int transferOut, int transferIn, int stat, const QString errorMsg );

    void setLocalLibrary( int count, int appended, int removed, int updated, int stat, const QString errorMsg );
  };

#endif // SDLIBRARYINDICATOR_H
