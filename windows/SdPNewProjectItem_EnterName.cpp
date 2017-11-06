/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPNewProjectItem_EnterName.h"
#include <QVBoxLayout>

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



void SdPNewProjectItem_EnterName::onTextChanged(const QString name)
  {
  if( name.isEmpty() ) {
    mUnical->setText( tr("<font color=\"blue\">Name is empty. You must enter correct name at least one symbol.</font>") );
    mValid = false;
    }
  else if( mProject->isNameUsed(name) ) {
    mUnical->setText( tr("<font color=\"red\">This name already exist. Enter another name.</font>") );
    mValid = false;
    }
  else {
    mUnical->setText( tr("Name is correct.") );
    mValid = true;
    }
  }



bool SdPNewProjectItem_EnterName::validatePage()
  {
  if( mValid )
    (*mItemPtr)->setTitle( mName->text() );
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
