/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdGuiderCapture.h"
#include "SdGuiderFile.h"
#include <QGuiApplication>
#include <QScreen>
#include <QPixmap>
#include <QWidget>
#include <QFileDialog>
#include <QCursor>

SdGuiderCapture::SdGuiderCapture(QWidget *main, QObject *parent) :
  QObject(parent),
  mFile(nullptr),
  mMainWindow(main)
  {

  connect( &mTimer, &QTimer::timeout, this, [this] () {
    if( mFile ) {
      //Capture current screen and append it to file
      QPixmap pix = QGuiApplication::primaryScreen()->grabWindow( mMainWindow->winId() );
      mFile->addImage( pix.toImage(), mTime );
      mFile->addCursor( QGuiApplication::mouseButtons(), QCursor::pos( QGuiApplication::primaryScreen() ), mTime );
      //Next time
      mTime++;
      //If movie too long we automatic stop capture
      if( mTime > 6000 )
        captureStop();
      }
    });
  }




void SdGuiderCapture::captureInit()
  {
  captureStop();

  mFile = new SdGuiderFile;
  mTime = 0;
  mTimer.start(100);
  }





void SdGuiderCapture::captureStop()
  {
  if( mFile ) {
    mTimer.stop();
    mFile->addStop( mTime );
    QString fname = QFileDialog::getSaveFileName( mMainWindow, tr("File for captured movie save") );
    if( !fname.isEmpty() ) {
      mFile->save( fname );
      }
    delete mFile;
    mFile = nullptr;
    }
  }
