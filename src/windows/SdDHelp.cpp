/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog with help page. Used to display help to dialogs.
*/
#include "SdDHelp.h"
#include "SdWHelp.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>

SdDHelp::SdDHelp(const QString &helpPage, QWidget *parent) :
  QDialog( parent )
  {
  resize( 600, 400 );
  if( parent ) {
    QPoint p = parent->pos();
    QSize s = parent->size();
    move(p.x()+s.width(), p.y());
    }
  //Construct help window
  setWindowTitle( tr("Dialog help") );
  QVBoxLayout *box = new QVBoxLayout();
  QToolBar *bar = new QToolBar();
  box->addWidget( bar );

  mWHelp = new SdWHelp();

  QToolButton *tool = new QToolButton();
  tool->setToolTip( tr("Help contents") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/helpHome.png")) );
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, mWHelp, &SdWHelp::contents );

  tool = new QToolButton();
  tool->setToolTip( tr("Base page") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/helpFirst.png")) );
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, this, [this,helpPage] () { mWHelp->helpTopic( helpPage ); });

  tool = new QToolButton();
  tool->setToolTip( tr("Previous page") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/helpPrevious.png")) );
  tool->setEnabled(false);
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, mWHelp, &SdWHelp::backward );
  connect( mWHelp, &SdWHelp::backwardAvailable, tool, &QToolButton::setEnabled );

  tool = new QToolButton();
  tool->setToolTip( tr("Next page") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/helpNext.png")) );
  tool->setEnabled(false);
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, mWHelp, &SdWHelp::forward );
  connect( mWHelp, &SdWHelp::forwardAvailable, tool, &QToolButton::setEnabled );

  box->addWidget( mWHelp );
  QDialogButtonBox *but = new QDialogButtonBox(QDialogButtonBox::Close);
  box->addWidget( but );
  setLayout( box );

  connect( but->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &QDialog::close );
  //Show help page
  mWHelp->helpTopic( helpPage );
  }




void SdDHelp::help(const QString &helpPage, QWidget *parent)
  {
  SdDHelp hlpDlg( helpPage, parent );
  hlpDlg.exec();
  }
