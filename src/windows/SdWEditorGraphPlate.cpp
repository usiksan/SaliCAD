/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph editor for pcb layout
*/
#include "SdWEditorGraphPlate.h"
#include "SdDParamEditor.h"
#include "SdWCommand.h"
#include "SdDPads.h"
#include "SdPMasterList.h"
#include "SdPExportPlate_Gerber.h"
#include "SdPExportPlate_Assembler.h"
#include "SdPExport_Bom.h"
#include "SdPExport_Dxf.h"
#include "SdDRuleEditor.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphPartImp.h"
#include "modes/SdModeCRoadEnter.h"
#include "modes/SdModeCRoadMove.h"
#include "modes/SdModeCPartPlace.h"
#include "modes/SdModeCPartHighlight.h"
#include "modes/SdModeCPolygonEnter.h"
#include "modes/SdModeSelect.h"
#include "modes/SdModeCPlateIdentMove.h"
#include "modes/SdModeCPlateValueMove.h"
#include "modes/SdModeCPartImp.h"
#include "modes/SdModeCViaEnter.h"
#include "modes/SdModeCOriginPlate.h"
#include "modes/SdModeCTraceDelete.h"

#include <QDebug>
#include <QProgressDialog>
#include <QWizard>
#include <QMessageBox>
#include <QInputDialog>

SdWEditorGraphPlate::SdWEditorGraphPlate(SdPItemPlate *pcb, QWidget *parent) :
  SdWEditorGraph( pcb, parent ),
  mPlate(pcb),
  mRuleErrorIndex(-1) //Current rule error index
  {
  mGrid.set( 2500, 2500 );
  mScale.scaleSet( 0.01 );
  }




//Update tracing status info: unconnected nets and rule errors
void SdWEditorGraphPlate::updateTracingInfo()
  {
  SdPulsar::sdPulsar->emitSetTracingStatus( mPlate->unconnectedNetCount(), mPlate->ruleErrorsCount() );
  }




SdProjectItem *SdWEditorGraphPlate::getProjectItem() const
  {
  return mPlate;
  }



void SdWEditorGraphPlate::cmEditUndo()
  {
  //Perform default operations
  SdWEditorGraph::cmEditUndo();

  //Rebuild rat net
  mPlate->setDirtyRatNet();
  }



void SdWEditorGraphPlate::cmModeComponent()
  {
  modeSet( new SdModeCPartImp( this, getProjectItem() ) );
  }




void SdWEditorGraphPlate::cmNetSetup()
  {
  }

void SdWEditorGraphPlate::cmModeDisconnect()
  {
  }

void SdWEditorGraphPlate::cmModeLink()
  {
  }




void SdWEditorGraphPlate::cmModeMovePart()
  {
  modeSet( new SdModeCPartPlace( this, getProjectItem() ) );
  }




void SdWEditorGraphPlate::cmModePartHighlight()
  {
  modeSet( new SdModeCPartHighlight( this, getProjectItem() )  );
  }






void SdWEditorGraphPlate::cmModeEditWire()
  {
  }




void SdWEditorGraphPlate::cmModeRoadEnter()
  {
  modeSet( new SdModeCRoadEnter( this, getProjectItem() )  );
  }




void SdWEditorGraphPlate::cmModeRoadMove()
  {
  modeSet( new SdModeCRoadMove( this, getProjectItem() )  );
  }




void SdWEditorGraphPlate::cmModePolygon()
  {
  modeSet( new SdModeCPolygonEnter( this, getProjectItem() )  );
  }




void SdWEditorGraphPlate::cmModeTraceDelete()
  {
  modeSet( new SdModeCTraceDelete( this, getProjectItem() )  );
  }




void SdWEditorGraphPlate::cmModePad()
  {
  }



void SdWEditorGraphPlate::onActivateEditor()
  {
  SdWEditorGraph::onActivateEditor();

  //Activate menu
  SdWCommand::cmMenuRules->setVisible(true);
  SdWCommand::cmMenuInsertPcb->setVisible(true);
  SdWCommand::cmViewMirror->setChecked( mMirrorHorz );

  //Activate tool bar
  SdWCommand::barPcb->show();

  //Update unconnect information
  updateTracingInfo();
  }




void SdWEditorGraphPlate::cmPads()
  {
  SdDPads pads( mPlate, mPlate->getPadAssociationName(), mPlate->getPadMap(), this );
  if( pads.exec() ) {
    //Setup pads
    mPlate->setPadAssociation( pads.getAssociationName(), pads.getPadMap(), mPlate->getUndo() );
    //Set project dirty with pads changed
    dirtyProject();
    //Update
    dirtyCashe();
    update();
    }
  }




void SdWEditorGraphPlate::cmPcbStratum()
  {
  bool ok;
  int c = QInputDialog::getInt( this, tr("PCB signal layer count"), tr("Enter PCB signal layer count:"), mPlate->stratumCount(), 1, 30, 1, &ok );
  if( ok )
    mPlate->setStratumCount( c );
  }




void SdWEditorGraphPlate::cmRulesCheck()
  {
  QProgressDialog progress( tr("Checking rules..."), tr("Abort check"), 0, 10 * mPlate->stratumCount(), this );
  progress.setWindowModality(Qt::WindowModal);
  int count = 0;
  //Check rules
  mPlate->checkRules( [&count, &progress] () -> bool {
                        progress.setValue(++count);
                        return progress.wasCanceled();
                      });
  //Set show rule errors
  SdWCommand::cmShowRuleErrors->setChecked( sdEnvir->mShowRuleErrors = true );
  //Update rat-net
  mPlate->setDirtyRatNet();
  //Update
  update();
  }




void SdWEditorGraphPlate::cmRulesErrorNext()
  {
  int count = mPlate->ruleErrorsCount();
  mRuleErrorIndex++;
  if( mRuleErrorIndex >= count )
    mRuleErrorIndex = 0;
  if( mRuleErrorIndex >= 0 && mRuleErrorIndex < count )
    originSet( mPlate->ruleError(mRuleErrorIndex).center() );
  }




//Edit properties of selected objects
void SdWEditorGraphPlate::cmEditProperties()
  {
  //Only for selecting mode
  if( mMode == mSelect && mSelect != nullptr ) {
    //Find first symbol implement
    SdObject *partImp = nullptr;
    mSelect->getFragment()->forEach( dctPartImp, [&partImp] (SdObject *obj) -> bool {
      partImp = obj;
      return false;
      });
    //Use symImp params for init param editor dialog
    SdPtr<SdGraphPartImp> part(partImp);
    if( part.isValid() ) {
      SdDParamEditor editor( tr("Component params"), part->paramTable(), getProject(), true, false, this );
      if( editor.exec() ) {
        //Change params for all selected items
        SdUndo *undo = getProject()->getUndo();
        undo->begin( tr("Param change"), getProjectItem(), false );
        mSelect->getFragment()->forEach( dctPartImp, [&editor,undo] (SdObject *obj) -> bool {
          SdPtr<SdGraphPartImp> imp(obj);
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




void SdWEditorGraphPlate::cmModePartSelect(QStringList list)
  {
  //Set focus to current edit window
  setFocus();
  if( modeGet() )
    modeGet()->partSelect( list );
  }




void SdWEditorGraphPlate::cmModeViaEnter()
  {
  modeSet( new SdModeCViaEnter( this, getProjectItem() )  );
  }





void SdWEditorGraphPlate::cmModeOrigin()
  {
  modeSet( new SdModeCOriginPlate( this, mPlate, sdEnvir->mPartPinSize ) );
  }




//Show dialog with rule editor
void SdWEditorGraphPlate::cmRulesEdit()
  {
  SdDRuleEditor edit( mPlate, this );
  if( edit.exec() ) {
    //Reset current mode to reread rules
    if( modeGet() )
      modeGet()->reset();
    dirtyCashe();
    dirtyProject();
    update();
    }
  }




void SdWEditorGraphPlate::cmRenumeration()
  {
  //Perform renumeration of current pcb
  mPlate->renumeration();
  //Reset current mode
  if( modeGet() )
    modeGet()->reset();

  dirtyCashe();
  dirtyProject();
  update();
  }





//Move component reference
void SdWEditorGraphPlate::cmModeReferenceMove()
  {
  modeSet( new SdModeCPlateIdentMove( this, getProjectItem() )  );
  }




//Move component value
void SdWEditorGraphPlate::cmModeValueMove()
  {
  modeSet( new SdModeCPlateValueMove( this, getProjectItem() )  );
  }





void SdWEditorGraphPlate::cmFileExport()
  {
  //Wizard
  QWizard wizard(this);
  //Fill it with pages
  SdPMasterList *master = new SdPMasterList( tr("Export plate"), tr("Select export master"), &wizard );
  wizard.setPage( 0,   master );
  wizard.setPage( 1,   new SdPExportPlate_Gerber( this, mPlate, 1, master, &wizard) );
  wizard.setPage( 2,   new SdPExportPlate_Assembler( this, mPlate, 2, master, &wizard) );
  wizard.setPage( 3,   new SdPExport_Bom( mPlate, 3, master, &wizard) );
  wizard.setPage( 4,   new SdPExport_Dxf( mPlate, 4, master, &wizard) );
  wizard.exec();
  }
