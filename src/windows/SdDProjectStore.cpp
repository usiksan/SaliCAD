/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Project store to library dialog

  Allow enter project name in library and project category. On ok pressed project writting to library
*/
#include "SdDProjectStore.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdProjectItem.h"
#include "objects/SdTime2x.h"
#include "SdDHelp.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>


SdDProjectStore::SdDProjectStore( const QString name, SdProject *prj, QWidget *parent) :
  QDialog( parent ),
  mProject(prj)
  {
  setWindowTitle( tr("Enter library name for project") );

  QVBoxLayout *vlay = new QVBoxLayout();
  vlay->addWidget( mUnical = new QLabel() );
  vlay->addWidget( mName = new QLineEdit() );
  mName->setText( name );

  QDialogButtonBox *box = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help );
  connect( box, &QDialogButtonBox::accepted, this, &SdDProjectStore::accept );
  connect( box, &QDialogButtonBox::rejected, this, &SdDProjectStore::reject );
  vlay->addWidget( box );

  setLayout( vlay );

  connect( mName, &QLineEdit::textChanged, this, &SdDProjectStore::onTextChanged );
  connect( box->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDProjectStore.htm"), this );
    } );
  }






void SdDProjectStore::onTextChanged(const QString name)
  {
  if( name.isEmpty() ) {
    mUnical->setText( tr("<font color=\"blue\">Name is empty. You must enter correct name at least one symbol.</font>") );
    }
  else if( SdObjectFactory::isContains( mProject->getType(), name, SdProjectItem::getDefaultAuthor() ) ) {
    mUnical->setText( tr("<font color=\"red\">This name already exist and will be overwritted! If You dont want to then enter another name.</font>") );
    }
  else {
    mUnical->setText( tr("Name is correct and free.") );
    }
  }





void SdDProjectStore::accept()
  {
  SdLibraryHeader hdr;
  hdr.mName = mName->text();
  if( hdr.mName.isEmpty() ) {
    //Name fail
    QMessageBox::warning( this, tr("Warning!"), tr("Library name of project is empty. Enter name to store project or press cancel.") );
    return;
    }
  hdr.mType = mProject->getType();   //Type of stored object
  hdr.mAuthor = SdProjectItem::getDefaultAuthor();    //Author who create object
  hdr.mTime   = SdTime2x::current();    //Object time creation
  hdr.mClass  = mProject->getClass();   //Object class
  hdr.mParamTable = mProject->paramTable();

  //Store project in library
  SdObjectFactory::insertObject( hdr, mProject->write() );

  //Close dialog
  QDialog::accept();
  }
