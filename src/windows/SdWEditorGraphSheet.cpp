/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph editor for schematic sheet
*/
#include "SdWEditorGraphSheet.h"
#include "SdDParamEditor.h"
#include "SdWCommand.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "SdPMasterList.h"
#include "SdPExport_Bom.h"
#include "SdPExport_Dxf.h"
#include "modes/SdModeCSymImp.h"
#include "modes/SdModeCNetWire.h"
#include "modes/SdModeCLinearArea.h"
#include "modes/SdModeCNetName.h"
#include "modes/SdModeWireDisconnect.h"
#include "modes/SdModeCBus.h"
#include "modes/SdModeCFragment.h"
#include "modes/SdModeTBrowseSheetPart.h"
#include "modes/SdModeSelect.h"
#include "modes/SdModeCSheetIdentMove.h"
#include "modes/SdModeCSheetValueMove.h"
#include "windows/SdDExpressionEdit.h"

#include <QMessageBox>
#include <QDebug>

SdWEditorGraphSheet::SdWEditorGraphSheet(SdPItemSheet *sch, QWidget *parent) :
  SdWEditorGraph( sch, parent ),
  mSheet(sch)
  {
  mGrid.set( 250, 250 );
  }




SdProjectItem *SdWEditorGraphSheet::getProjectItem() const
  {
  return mSheet;
  }




void SdWEditorGraphSheet::onActivateEditor()
  {
  SdWEditorGraph::onActivateEditor();

  //Activate menu
  SdWCommand::cmMenuInsertSheet->setVisible(true);

  //Activate tool bar
  SdWCommand::barSheet->show();
  }





//Fragment insertion mode
void SdWEditorGraphSheet::cmModeFragment()
  {
  modeSet( new SdModeCFragment( this, mSheet ) );
  }


//Component insertion mode
void SdWEditorGraphSheet::cmModeComponent()
  {
  modeSet( new SdModeCSymImp( this, mSheet ) );
  }



//Sheet net wire insertion mode
void SdWEditorGraphSheet::cmModeNet()
  {
  modeSet( new SdModeCNetWire( this, mSheet ) );
  }




//Sheet bus insertion mode
void SdWEditorGraphSheet::cmModeBus()
  {
  modeSet( new SdModeCBus( this, mSheet )  );
  }




//Sheet net wire disconnect from pin mode
void SdWEditorGraphSheet::cmModeDisconnect()
  {
  modeSet( new SdModeWireDisconnect( this, mSheet )  );
  }




//Sheet pcb area insertion mode
void SdWEditorGraphSheet::cmModePcbArea()
  {
  modeSet( new SdModeCLinearArea( this, mSheet ) );
  }



//Sheet net name insertion mode
void SdWEditorGraphSheet::cmModeNetName()
  {
  modeSet( new SdModeCNetName( this, mSheet ) );
  }





//Browse part implement in sheet
void SdWEditorGraphSheet::cmModeBrowse(SdProjectItem *plate)
  {
  modeCall( new SdModeTBrowseSheetPart( this, mSheet, plate ) );
  }




//Edit properties of selected objects
void SdWEditorGraphSheet::cmEditProperties()
  {
  //Only for selecting mode
  if( mMode == mSelect && mSelect != nullptr ) {
    //Find first symbol implement
    SdObject *symImp = nullptr;
    mSelect->getFragment()->forEach( dctSymImp, [&symImp] (SdObject *obj) -> bool {
      symImp = obj;
      return false;
      });
    //Use symImp params for init param editor dialog
    SdPtr<SdGraphSymImp> sym(symImp);
    if( sym.isValid() ) {
      SdDParamEditor editor( tr("Component params"), sym->paramTable(), getProject(), true, false, this );
      if( editor.exec() ) {
        //Change params for all selected items
        SdUndo *undo = getProject()->getUndo();
        undo->begin( tr("Param change"), getProjectItem(), false );
        mSelect->getFragment()->forEach( dctSymImp, [&editor,undo] (SdObject *obj) -> bool {
          SdPtr<SdGraphSymImp> imp(obj);
          if( imp.isValid() )
            imp->paramTableSet( editor.paramTable(), undo, nullptr );
          return true;
          });
        dirtyProject();
        }
      }
    else QMessageBox::warning( this, tr("Error!"), tr("Parameters edit available only for component. No component selected. Select components and try again.") );
    }
  }



void SdWEditorGraphSheet::cmModeNetList()
  {
  }




void SdWEditorGraphSheet::cmRenumeration()
  {
  //Perform renumeration
  getProject()->renumeration();

  //Update visual
  dirtyCashe();
  dirtyProject();
  update();
  }





//Move component reference
void SdWEditorGraphSheet::cmModeReferenceMove()
  {
  modeSet( new SdModeCSheetIdentMove( this, mSheet ) );
  }





//Move component value
void SdWEditorGraphSheet::cmModeValueMove()
  {
  modeSet( new SdModeCSheetValueMove( this, mSheet ) );
  }




//!
//! \brief cmExpressionEdit Shows expression edit dialog with expression test capabilities
//!
void SdWEditorGraphSheet::cmExpressionEdit()
  {
  SdDExpressionEdit expressionEdit( mSheet->expressionGet(), this );
  if( expressionEdit.exec() ) {
    //Setup expression
    mSheet->expressionSetText( expressionEdit.textGet() );
    //Set project dirty with expression changes
    dirtyProject();
    }
  }





//Export command
void SdWEditorGraphSheet::cmFileExport()
  {
  //Wizard
  QWizard wizard(this);
  //Fill it with pages
  SdPMasterList *master = new SdPMasterList( tr("Export sheet"), tr("Select export master"), &wizard );
  wizard.setPage( 0,   master );
  wizard.setPage( 1,   new SdPExport_Bom( mSheet, 1, master, &wizard) );
  wizard.setPage( 2,   new SdPExport_Dxf( mSheet, 2, master, &wizard) );
  wizard.exec();
  }



