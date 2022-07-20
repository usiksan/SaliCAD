/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Page of creation master with enter ProjectItem name and category selection
*/
#include "SdConfig.h"
#include "SdPNewProjectItem_EnterName.h"
#include "SdWCategoryList.h"
#include "library/SdLibraryStorage.h"

#include <QVBoxLayout>
#include <QMessageBox>

SdPNewProjectItem_EnterName::SdPNewProjectItem_EnterName(SdProjectItemPtr *item, SdProject *prj, QWidget *parent, bool noRename) :
  QWizardPage(parent),
  mItemPtr(item),
  mProject(prj),
  mValid(true)
  {
  setTitle( tr("Enter name for object") );
  setSubTitle( tr("Enter unical name inside project for object") );

  QVBoxLayout *vlay = new QVBoxLayout();
  vlay->addWidget( mUnical = new QLabel() );
  vlay->addWidget( mName = new QLineEdit() );
  if( noRename ) mName->setReadOnly(true);
  vlay->addWidget( new QLabel(tr("Element category")) );
  vlay->addWidget( mCategory = new QLineEdit() );
  mCategory->setReadOnly(true);
  mCategoryList = new SdWCategoryList(nullptr);
  vlay->addWidget( mCategoryList );
  if( (*item) != nullptr ) {
    QString path = (*item)->paramGet( QString(stdParamCategory) );
    mCategoryList->cmLocate(path);
    mCategory->setText( path );

    //connect( mCategoryList, &SdWCategoryList::category, this, [this] ( const QString path) { (*mItemPtr)->paramSet( QString(stdParamCategory), path, nullptr ); } );
    }

  connect( mCategoryList, &SdWCategoryList::category, mCategory, &QLineEdit::setText );

  setLayout( vlay );

  connect( mName, &QLineEdit::textChanged, this, &SdPNewProjectItem_EnterName::onTextChanged );
  }





//On changed ProjectItem name
void SdPNewProjectItem_EnterName::onTextChanged(const QString name)
  {
  if( name.isEmpty() ) {
    mUnical->setText( tr("<font color=\"blue\">Name is empty. You must enter correct name at least one symbol.</font>") );
    mValid = false;
    }
  else if( mProject->isNameUsed( name, (*mItemPtr)->getClass() ) || SdLibraryStorage::instance()->cfObjectContains( headerUid( (*mItemPtr)->getType(), name, SdProjectItem::getDefaultAuthor() ) ) ) {
    mUnical->setText( tr("<font color=\"red\">This name already exist. Enter another name or this override existing.</font>") );
    mValid = true;
    }
  else {
    mUnical->setText( tr("Name is correct and free.") );
    mValid = true;
    }
  }



bool SdPNewProjectItem_EnterName::validatePage()
  {
  if( mValid ) {
    if( SdLibraryStorage::instance()->cfObjectContains( headerUid( (*mItemPtr)->getType(), mName->text(), SdProjectItem::getDefaultAuthor() ) ) ) {
      if( QMessageBox::question( this, tr("Warning!"), tr("Object with this name and author already exist in base. Overwrite existing object?"),
                                 QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) != QMessageBox::Yes )
        return false;
      }
    (*mItemPtr)->setTitle( mName->text(), tr("Set object title") );
    (*mItemPtr)->paramSet( QString(stdParamCategory), mCategory->text(), nullptr );
    }
  return mValid;
  }



bool SdPNewProjectItem_EnterName::isComplete() const
  {
  return true;
  }



int SdPNewProjectItem_EnterName::nextId() const
  {
  return -1;
  }




void SdPNewProjectItem_EnterName::initializePage()
  {
  if( *mItemPtr ) {
    mName->setText( (*mItemPtr)->getTitle() );
    mCategory->setText( (*mItemPtr)->paramGet(QString(stdParamCategory)) );
    }
  }



//Enter new object name and category or category only
void SdPNewProjectItem_EnterName::nameProjectItem(SdProjectItemPtr *item, SdProject *prj, QWidget *parent, bool noRename)
  {
  QWizard wizard(parent);

  wizard.setPage( 1, new SdPNewProjectItem_EnterName( item, prj, &wizard, noRename ) );

  wizard.exec();
  }
