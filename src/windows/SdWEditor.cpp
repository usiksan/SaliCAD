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
#include "objects/SdPItemSymbol.h"
#include "objects/SdPItemComponent.h"
#include "library/SdLibraryStorage.h"
#include "SdWEditor.h"
#include "SdWCommand.h"


#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>

SdWEditor::SdWEditor(QWidget *parent) :
  QAbstractScrollArea(parent),
  mRecentlyIndex(SD_MAX_EDITORS)
  {

  }




SdProject *SdWEditor::getProject()
  {
  if( getProjectItem() )
    return getProjectItem()->getProject();
  return nullptr;
  }




void SdWEditor::dirtyProject()
  {
  //Set project dirty
  getProject()->setDirty();
  }




QString SdWEditor::getTitle()
  {
  return getProjectItem()->getTitle() + (getProjectItem()->isEditEnable() ? QStringLiteral(" [*]") : QStringLiteral(" [-]"));
  }




QString SdWEditor::getToolTip()
  {
  return getProjectItem()->getToolTip();
  }




QString SdWEditor::getIconName()
  {
  return getProjectItem()->getIconName();
  }





//Update status of undo and redo
void SdWEditor::updateUndoRedoStatus()
  {
  bool undo = getProject()->getUndo()->isUndoPresent();
  bool redo = getProject()->getUndo()->isRedoPresent();
  SdWCommand::cmEditUndo->setEnabled( undo );
  SdWCommand::cmEditRedo->setEnabled( redo );
  //qDebug() << "update undo" << undo << redo;
  }





void SdWEditor::onActivateEditor()
  {
  updateUndoRedoStatus();

  SdWCommand::cmFilePrint->setEnabled(true);

  bool ena3d = getProjectItem() != nullptr && getProjectItem()->is3dAllowed();
  SdWCommand::cmView3d->setEnabled( ena3d );
  SdWCommand::cmViewMirror->setEnabled( ena3d );

  //Highlight item
  if( is3d() )
    SdPulsar::sdPulsar->emitHighlightItem3d( getProjectItem() );
  else
    SdPulsar::sdPulsar->emitHighlightItem( getProjectItem() );
  }




void SdWEditor::cmObjectEditDisable()
  {
  if( getProjectItem() ) {
    SdProjectItem *item = getProjectItem();
    //Check if object already in library
    bool presenceInLibrary = SdLibraryStorage::instance()->cfObjectContains( item->hashUidName() );
    getProjectItem()->setEditEnable( false, tr("Object edit disable") );


    //For symbols which yet not in library we query for creation default component
    SdPtr<SdPItemSymbol> sym(item);
    SdPItemComponent *comp = nullptr;
    if( sym.isValid() && !presenceInLibrary ) {
      //Query for creation default component
      QStringList items;
      items << tr("with default part and edit it") << tr("without part and edit it");
      QInputDialog dlg(this);
      dlg.setOptions( QInputDialog::UseListViewForComboBoxItems );
      dlg.setCancelButtonText( tr("No") );
      dlg.setOkButtonText( tr("Create") );
      dlg.setComboBoxEditable(false);
      dlg.setComboBoxItems( items );
      dlg.setInputMode( QInputDialog::TextInput );
      dlg.setLabelText( tr("Do You want to create default component:") );
      if( dlg.exec() ) {
        if( dlg.textValue() == items.at(0) )
          //Create default component with default part and fix it
          comp = sdCreateDefaultComponent( sym.ptr(), true );
        else
          //Create default component without part and edit it
          comp = sdCreateDefaultComponent( sym.ptr(), false );
        }
      }

    //Close this editor (viewer)
    SdPulsar::sdPulsar->emitCloseEditView( getProjectItem() );
    //Open new item with disabled edit status
    SdPulsar::sdPulsar->emitActivateItem( item );

    //If component present then open it for edit
    if( comp )
      SdPulsar::sdPulsar->emitActivateItem( comp );
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
      int r = QMessageBox::question( this, tr("Warning"), tr("Object was created by another author '%1'. Change to your name?").arg(getProjectItem()->getAuthorKey()) );
      //Alien author object edit not allowed
      if( r == QMessageBox::No ) return;
      //Check if name unical for this author
      getProjectItem()->setUnicalTitle( tr("Set unical object name") );
      item = getProjectItem()->setEditEnable( true, QString() );
      }
    else {
      //Request for database removing
      int r = QMessageBox::question( this, tr("Warning"), tr("Remove '%1' from database?").arg(getProjectItem()->getTitle()) );
      if( r == QMessageBox::Yes )
        //Remove from database
        SdLibraryStorage::instance()->cfObjectDelete( getProjectItem() );
      item = getProjectItem()->setEditEnable( true, tr("Object edit enable") );
      }
    //Close this editor (viewer)
    SdPulsar::sdPulsar->emitCloseEditView( getProjectItem() );
    //Open new item with edit status
    SdPulsar::sdPulsar->emitActivateItem( item );
    }
  }



