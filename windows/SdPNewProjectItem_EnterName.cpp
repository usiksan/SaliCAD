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
#include "objects/SdObjectFactory.h"

#include <QVBoxLayout>
#include <QMessageBox>

SdPNewProjectItem_EnterName::SdPNewProjectItem_EnterName(SdProjectItemPtr *item, SdProject *prj, QWidget *parent) :
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
  else if( mProject->isNameUsed( name, (*mItemPtr)->getClass() ) || SdObjectFactory::isContains( (*mItemPtr)->getType(), name, SdProjectItem::getDefaultAuthor() ) ) {
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
