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
      SdDParamEditor editor( tr("Component params"), sym->paramTable(), getProject(), true, this );
      if( editor.exec() ) {
        //Change params for all selected items
        SdUndo *undo = getProject()->getUndo();
        undo->begin( tr("Param change"), getProjectItem() );
        mSelect->getFragment()->forEach( dctSymImp, [&editor,undo] (SdObject *obj) -> bool {
          SdPtr<SdGraphSymImp> imp(obj);
          if( imp.isValid() )
            imp->paramTableSet( editor.paramTable(), undo, nullptr );
          return true;
          });
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



