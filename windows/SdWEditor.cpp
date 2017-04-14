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
#include "objects/SdProject.h"
#include "SdWEditor.h"
#include "SdWCommand.h"

SdWEditor::SdWEditor(QWidget *parent) : QAbstractScrollArea(parent)
  {

  }

SdProject *SdWEditor::getProject()
  {
  return getProjectItem()->getProject();
  }

void SdWEditor::dirtyProject()
  {
  getProjectItem()->getProject()->setDirty();
  }

QString SdWEditor::getTitle()
  {
  return getProjectItem()->getTitle();
  }

QString SdWEditor::getIconName()
  {
  return getProjectItem()->getIconName();
  }



void SdWEditor::onActivateEditor()
  {
  SdWCommand::cmEditUndo->setEnabled( getProject()->getUndo()->isUndoPresent() );
  SdWCommand::cmEditRedo->setEnabled( getProject()->getUndo()->isRedoPresent() );
  }



