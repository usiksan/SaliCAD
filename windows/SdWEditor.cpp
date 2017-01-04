/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "objects/SdProjectItem.h"
#include "SdWEditor.h"

SdWEditor::SdWEditor(QWidget *parent) : QWidget(parent)
  {

  }

QString SdWEditor::getTitle()
  {
  return getProjectItem()->getTitle();
  }

QString SdWEditor::getIconName()
  {
  return getProjectItem()->getIconName();
  }



void SdWEditor::paintEvent(QPaintEvent *)
  {
  }
