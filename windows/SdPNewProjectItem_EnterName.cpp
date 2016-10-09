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
    mUnical->setText( tr("Name is empty. You must enter correct name at least one symbol.") );
    mValid = false;
    }
  else if( mProject->isContains(name) ) {
    mUnical->setText( tr("This name already exist in project. Enter another name.") );
    mValid = false;
    }
  else {
    mUnical->setText( tr("Name is correct.") );
    mValid = true;
    }
  }



bool SdPNewProjectItem_EnterName::validatePage()
  {
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
