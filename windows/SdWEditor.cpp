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





//Update status of undo and redo
void SdWEditor::updateUndoRedoStatus()
  {
  SdWCommand::cmEditUndo->setEnabled( getProject()->getUndo()->isUndoPresent() );
  SdWCommand::cmEditRedo->setEnabled( getProject()->getUndo()->isRedoPresent() );
  }





void SdWEditor::onActivateEditor()
  {
  updateUndoRedoStatus();

  SdWCommand::cmFilePrint->setEnabled(true);
  }




void SdWEditor::cmObjectEditDisable()
  {
  if( getProjectItem() ) {
    SdProjectItem *item = getProjectItem();
    getProjectItem()->setEditEnable( false, tr("Object edit disable") );
    //Close this editor (viewer)
    SdPulsar::sdPulsar->emitCloseEditView( getProjectItem() );
    //Open new item with edit status
    SdPulsar::sdPulsar->emitActivateItem( item );
    }
  }






////Show dialog with error msg
//QMessageBox::StandardButton SdWEditor::msgError(QString msg, QMessageBox::StandardButton buttons)
//  {
//  return QMessageBox::warning( this, tr("Error!"), msg, buttons );
//  }





////Show dialog with warning msg
//QMessageBox::StandardButton SdWEditor::msgWarning(QString msg, QMessageBox::StandardButton buttons)
//  {
//  return QMessageBox::warning( this, tr("Warning!"), msg, buttons );
//  }





////Show dialog with info msg
//QMessageBox::StandardButton SdWEditor::msgInfo(QString msg, QMessageBox::StandardButton buttons)
//  {
//  return QMessageBox::information( this, tr("Information!"), msg, buttons );
//  }








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
    SdPulsar::sdPulsar->emitCloseEditView( getProjectItem() );
    //Open new item with edit status
    SdPulsar::sdPulsar->emitActivateItem( item );
    }
  }



