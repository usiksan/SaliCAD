/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdDRuleEditor.h"
#include "ui_SdDRuleEditor.h"

#include "objects/SdPItemPlate.h"
#include "objects/SdProject.h"

SdDRuleEditor::SdDRuleEditor(SdPItemPlate *plate, QWidget *parent) :
  QDialog(parent),
  mPlate(plate),
  ui(new Ui::SdDRuleEditor)
  {
  ui->setupUi(this);

  //Fill net list
  QStringList netList = mPlate->getProject()->netList();
  ui->mNetList->addItems( netList );


  }

SdDRuleEditor::~SdDRuleEditor()
  {
  delete ui;
  }
