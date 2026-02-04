
#include "SdLibraryIndicator.h"

SdLibraryIndicator::SdLibraryIndicator()
  {
  mGlobalStorage = new QAction( QIcon(QString(":/pic/iconGlobalStorage.png")), QString{} );
  mPrivateCloud = new QAction( QIcon(QString(":/pic/iconPrivateCloud.png")), QString{} );
  mLocalLibrary = new QAction( QIcon(QString(":/pic/iconLocalLibrary.png")), QString{} );
  // QLabel  *mGlobalStorageIndicator;
  // QLabel  *mPrivateCloudIndicator;
  // QLabel  *mLocalLibraryIndicator;

  }

void SdLibraryIndicator::addToStatusBar(QStatusBar *sbar)
  {

  }

void SdLibraryIndicator::setGlobalStorage(int transferOut, int transferIn, SdLibraryIndicatorStatus stat, const QString errorMsg)
  {

  }

void SdLibraryIndicator::setPrivateCloud(int transferOut, int transferIn, SdLibraryIndicatorStatus stat, const QString errorMsg)
  {

  }

void SdLibraryIndicator::setLocalLibrary(int appended, int removed, int updated, SdLibraryIndicatorStatus stat, const QString errorMsg)
  {

  }
