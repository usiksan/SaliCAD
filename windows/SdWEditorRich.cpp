/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Rich text object editor.
*/
#include "SdWEditorRich.h"
#include "objects/SdProject.h"

#include <QVBoxLayout>

SdWEditorRich::SdWEditorRich(SdPItemRich *item, QWidget *parent) :
  SdWEditor( parent ),
  mRich( item ),
  mDirty(false)
  {
  QVBoxLayout *box = new QVBoxLayout();
  mTextEdit = new QTextEdit();
  box->addWidget( mTextEdit );
  setLayout( box );

  mTextEdit->setHtml( mRich->contens() );

  connect( mTextEdit, &QTextEdit::textChanged, this, &SdWEditorRich::onTextChanged );
  }




SdWEditorRich::~SdWEditorRich()
  {
  if( mDirty )
    mRich->setContens( mTextEdit->toHtml() );
  }





SdProjectItem *SdWEditorRich::getProjectItem() const
  {
  return mRich;
  }





void SdWEditorRich::onTextChanged()
  {
  mDirty = true;
  getProject()->setDirty();
  }
