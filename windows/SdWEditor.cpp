/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Common base class for any editors.
*/
#include "objects/SdProjectItem.h"
#include "objects/SdProject.h"
#include "objects/SdPulsar.h"
#include "SdWEditor.h"
#include "SdWCommand.h"

#include <QMessageBox>

SdWEditor::SdWEditor(QWidget *parent) :
  QAbstractScrollArea(parent),
  mRecentlyIndex(SD_MAX_EDITORS)
  {

  }




SdProject *SdWEditor::getProject()
  {
  return getProjectItem()->getProject();
  }




void SdWEditor::dirtyProject()
  {
  //Set project dirty
  getProjectItem()->getProject()->setDirty();
  }




QString SdWEditor::getTitle()
  {
  return getProjectItem()->getTitle() + (getProjectItem()->isEditEnable() ? QStringLiteral(" [*]") : QStringLiteral(" [-]"));
  }




QString SdWEditor::getIconName()
  {
  return getProjectItem()->getIconName();
  }





void SdWEditor::onActivateEditor()
  {
  SdWCommand::cmEditUndo->setEnabled( getProject()->getUndo()->isUndoPresent() );
  SdWCommand::cmEditRedo->setEnabled( getProject()->getUndo()->isRedoPresent() );

  SdWCommand::cmFilePrint->setEnabled(true);
  }




void SdWEditor::cmObjectEditDisable()
  {
  if( getProjectItem() ) {
    SdProjectItem *item = getProjectItem();
    getProjectItem()->setEditEnable( false, tr("Object edit disable") );
    //Close this editor (viewer)
    SdPulsar::pulsar->emitCloseEditView( getProjectItem() );
    //Open new item with edit status
    SdPulsar::pulsar->emitActivateItem( item );
    }
  }




void SdWEditor::cmObjectEditEnable()
  {
  if( getProjectItem() ) {
    SdProjectItem *item;
    //Check if author
    if( getProjectItem()->isAnotherAuthor() ) {
      int r = QMessageBox::question( this, tr("Warning"), tr("Object was created by another author '%1'. Change to your name?").arg(getProjectItem()->getAuthor()) );
      //Alien author object edit not allowed
      if( r == QMessageBox::No ) return;
      //Check if name unical for this author
      getProjectItem()->setUnicalTitle( tr("Set unical object name") );
      item = getProjectItem()->setEditEnable( true, QString() );
      }
    else
      item = getProjectItem()->setEditEnable( true, tr("Object edit enable") );
    //Close this editor (viewer)
    SdPulsar::pulsar->emitCloseEditView( getProjectItem() );
    //Open new item with edit status
    SdPulsar::pulsar->emitActivateItem( item );
    }
  }



