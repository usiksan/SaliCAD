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

SdDHelp::SdDHelp(const QString &helpPage, QWidget *parent) :
  QDialog( parent )
  {
  resize( 1200, 800 );
  //Construct help window
  setWindowTitle( tr("Dialog help") );
  QVBoxLayout *box = new QVBoxLayout();
  SdWHelp *whelp = new SdWHelp();
  box->addWidget( whelp );
  QDialogButtonBox *but = new QDialogButtonBox(QDialogButtonBox::Close);
  box->addWidget( but );
  setLayout( box );

  connect( but->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &QDialog::close );
  //Show help page
  whelp->helpTopic( helpPage );
  }




void SdDHelp::help(const QString &helpPage, QWidget *parent)
  {
  SdDHelp hlpDlg( helpPage, parent );
  hlpDlg.exec();
  }
