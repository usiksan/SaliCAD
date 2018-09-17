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

#include "SdPNewProjectItem_EnterName.h"
#include "SdWCategory.h"
#include "objects/SdObjectFactory.h"

#include <QVBoxLayout>
#include <QMessageBox>

SdPNewProjectItem_EnterName::SdPNewProjectItem_EnterName(SdProjectItemPtr *item, SdProject *prj, bool categoryOnly, QWidget *parent) :
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
  vlay->addWidget( new QLabel(tr("Element category")) );
  vlay->addWidget( mCategory = new QLineEdit() );
  if( (*item) != nullptr ) {
    mTagPath = (*item)->getTag();
    mCategory->setText( mTagPath );
    }
  SdWCategory *category = new SdWCategory();
  connect( category, &SdWCategory::categorySelected, mCategory, &QLineEdit::setText );
  connect( category, &SdWCategory::tagPathSelected, this, [this] ( const QString path) { mTagPath = path; } );
  vlay->addWidget( category );

  setLayout( vlay );

  if( categoryOnly )
    mName->setReadOnly(true);
  else
    connect( mName, &QLineEdit::textChanged, this, &SdPNewProjectItem_EnterName::onTextChanged );
  }





//On changed ProjectItem name
void SdPNewProjectItem_EnterName::onTextChanged(const QString name)
  {
  if( name.isEmpty() ) {
    mUnical->setText( tr("<font color=\"blue\">Name is empty. You must enter correct name at least one symbol.</font>") );
    mValid = false;
    }
  else if( mProject->isNameUsed(name) || SdObjectFactory::isContains( (*mItemPtr)->getType(), name, SdProjectItem::getDefaultAuthor() ) ) {
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
    if( SdObjectFactory::isContains( (*mItemPtr)->getType(), mName->text(), SdProjectItem::getDefaultAuthor() ) ) {
      if( QMessageBox::question( this, tr("Warning!"), tr("Object with this name and author already exist in base. Overwrite existing object?"),
                                 QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) != QMessageBox::Yes )
        return false;
      }
    (*mItemPtr)->setTitle( mName->text(), tr("Set object title") );
    (*mItemPtr)->setTag( mTagPath, tr("Set object tag") );
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
    }
  }
