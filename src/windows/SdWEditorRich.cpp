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

  //Register editor to item. When item will be save it will read contens from editor
  item->setEditor( mTextEdit );

  mTextEdit->setHtml( mRich->contens() );

  connect( mTextEdit, &QTextEdit::textChanged, this, &SdWEditorRich::onTextChanged );
  }




SdWEditorRich::~SdWEditorRich()
  {
  if( mDirty )
    mRich->setContens( mTextEdit->toHtml() );
  mRich->setEditor( nullptr );
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
