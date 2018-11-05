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
#include "SdDGuiderPlayer.h"
#include "SdWHelp.h"
#include "library/SvDir.h"

#include <QTextToSpeech>
#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QCoreApplication>
#include <QFile>
#include <QSettings>

static QTextToSpeech *speech;


SdDGuiderPlayer::SdDGuiderPlayer(const QString fname, QWidget *parent) :
  QDialog( parent ),
  mCurrentTime(0)
  {
  mFile.load( guiderPath() + fname );
  mFile.play(0);

  if( speech == nullptr )
    speech = new QTextToSpeech();
  mFile.mTiterChanged = [this] () { titerChanged(); };

  resize( 1000, 800 );

  //Construct help window
  setWindowTitle( tr("Guider") );
  QVBoxLayout *box = new QVBoxLayout();
  QToolBar *bar = new QToolBar();
  box->addWidget( bar );

  mView = new QLabel;

  QToolButton *tool = new QToolButton();
  tool->setToolTip( tr("Close") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/guiderEject.png")) );
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, this, &SdDGuiderPlayer::close );

  tool = new QToolButton();
  tool->setToolTip( tr("Restart") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/guiderBackward.png")) );
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, this, &SdDGuiderPlayer::cmPlayRestart );

  tool = new QToolButton();
  tool->setToolTip( tr("Fast backward") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/guiderFastBackward.png")) );
  bar->addWidget( tool );

  tool = new QToolButton();
  tool->setToolTip( tr("Fast forward") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/guiderFastForward.png")) );
  bar->addWidget( tool );

  tool = new QToolButton();
  tool->setToolTip( tr("Start play") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/guiderStart.png")) );
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, this, &SdDGuiderPlayer::cmPlayStart );
  connect( this, &SdDGuiderPlayer::sgPlay, tool, &QToolButton::setDisabled );

  tool = new QToolButton();
  tool->setToolTip( tr("Pause") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/guiderPause.png")) );
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, this, &SdDGuiderPlayer::cmPlayPause );
  connect( this, &SdDGuiderPlayer::sgPlay, tool, &QToolButton::setEnabled );

  tool = new QToolButton();
  tool->setToolTip( tr("Stop") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/guiderStop.png")) );
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, this, &SdDGuiderPlayer::cmPlayStop );
  connect( this, &SdDGuiderPlayer::sgPlay, tool, &QToolButton::setEnabled );


  box->addWidget( mView );
  mTiter = new QLabel();
  mTiter->setMinimumHeight( 80 );
  mTiter->setWordWrap(true);
  mTiter->setAlignment( Qt::AlignHCenter | Qt::AlignTop );
  setLayout( box );

  connect( &mTimer, &QTimer::timeout, this, &SdDGuiderPlayer::play );

  }




QString SdDGuiderPlayer::guiderPath()
  {
  return SdWHelp::helpPath() + QString("guider/");
  }






bool SdDGuiderPlayer::guiderExist(const QString fname)
  {
  return QFile::exists( guiderPath() + fname );
  }





void SdDGuiderPlayer::cmPlayRestart()
  {
  mCurrentTime = 0;
  mCurrentFrame = 0;
  cmPlayStart();
  }




void SdDGuiderPlayer::cmPlayStart()
  {
  mTimer.start(100);
  emit sgPlay(true);
  }




void SdDGuiderPlayer::cmPlayPause()
  {
  cmPlayStop();
  }




void SdDGuiderPlayer::cmPlayStop()
  {
  mTimer.stop();
  emit sgPlay(false);
  }




void SdDGuiderPlayer::play()
  {
  mCurrentTime++;
  while( mCurrentFrame < mFile.mFile.count() && mFile.mFile.at(mCurrentFrame).mTime < mCurrentTime )
    mFile.play( mCurrentFrame++ );
  mView->clear();
  mView->setPixmap( mFile.build() );
  if( mCurrentFrame >= mFile.mFile.count() )
    cmPlayStop();
  }




void SdDGuiderPlayer::titerChanged()
  {
  if( mFile.mTiterIndex >= 0 ) {
    QString val;
    if( mFile.mTiter.mContens.contains(mLanguage) )
      val = mFile.mTiter.mContens.value(mLanguage);
    else
      val = mFile.mTiter.mContens.value( QStringLiteral("en") );
    speech->say( val );
    mTiter->setText( QStringLiteral("<h1>%1</h1>").arg(val) );
    }
  else mTiter->clear();
  }


