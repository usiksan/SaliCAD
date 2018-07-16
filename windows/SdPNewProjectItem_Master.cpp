/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdPNewProjectItem_Master.h"
#include "SdPNewProjectItem.h"

//Master dialogs
#include "master/SdDMasterPartDoubleRect.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include <QSettings>
#include <QMessageBox>


SdPNewProjectItem_Master::SdPNewProjectItem_Master(SdProjectItemPtr *item, SdProject *prj, QWidget *parent) :
  QWizardPage(parent),
  mItemPtr(item),
  mProject(prj)
  {

  setTitle( tr("Creation new project object") );
  setSubTitle( tr("Select object type then object creation order") );

  QHBoxLayout *hlay = new QHBoxLayout();
  QVBoxLayout *vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Select master type") ) );
  mMasterType = new QListWidget();
  vlay->addWidget( mMasterType );
  hlay->addLayout( vlay );

  vlay = new QVBoxLayout();
  vlay->addWidget( mImage = new QLabel() );
  hlay->addLayout( vlay );

  vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Description")) );
  vlay->addWidget( mDescription = new QTextEdit() );
  mDescription->setReadOnly(true);
  hlay->addLayout( vlay );

  setLayout( hlay );

  connect( mMasterType, &QListWidget::currentRowChanged, this, [this] ( int row ) {
    if( row >= 0 ) {
      mDescription->setText( mDescriptions.at(row) );
      mImage->setPixmap( QPixmap(mImages.at(row)) );
      }
    } );
  }




bool SdPNewProjectItem_Master::validatePage()
  {
  int index = mMasterType->currentRow();
  if( index >= 0 )
    return mFuncs[index]( *mItemPtr, this );
  return false;
  }




int SdPNewProjectItem_Master::nextId() const
  {
  return SDP_NPI_NAME;
  }




void SdPNewProjectItem_Master::initializePage()
  {
  //Fill object classes
  switch( (*mItemPtr)->getClass() ) {
    case dctPart :
      addMaster( tr("Two pins part"), tr("Creates part with exact two pins and rectangle body (resistor, condensator and so on"),
                 QString(":/pic/partMasterDoubleRect.png"), [] ( SdProjectItem *item, QWidget *p ) -> bool {
        SdDMasterPartDoubleRect dlg( item, p );
        return dlg.exec();
        });
    }

  mMasterType->setCurrentRow( 0 );
  }




void SdPNewProjectItem_Master::addMaster(QString title, QString descr, QString img, SdMasterFun fun)
  {
  mMasterType->addItem( title );
  mDescriptions.append( descr );
  mImages.append(img);
  mFuncs.append(fun);
  }
