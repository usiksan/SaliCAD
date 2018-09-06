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
#include "SdWCommand.h"
#include "SdDPads.h"
#include "SdPMasterList.h"
#include "SdPExportPlate_Gerber.h"
#include "SdDRuleEditor.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "modes/SdModeCRoadEnter.h"
#include "modes/SdModeCPartPlace.h"
#include "modes/SdModeCPolygonEnter.h"

#include <QDebug>
#include <QProgressDialog>
#include <QWizard>

SdWEditorGraphPlate::SdWEditorGraphPlate(SdPItemPlate *pcb, QWidget *parent) :
  SdWEditorGraph( pcb, parent ),
  mPlate(pcb),
  mRuleErrorIndex(-1) //Current rule error index
  {
  mGrid.set( 2500, 2500 );
  mScale.scaleSet( 0.01 );
  }




SdProjectItem *SdWEditorGraphPlate::getProjectItem() const
  {
  return mPlate;
  }



void SdWEditorGraphPlate::cmModeComponent()
  {
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



void SdWEditorGraphPlate::cmModePlace()
  {
  }

void SdWEditorGraphPlate::cmModeEditWire()
  {
  }




void SdWEditorGraphPlate::cmModeRoadEnter()
  {
  modeSet( new SdModeCRoadEnter( this, getProjectItem() )  );
  }




void SdWEditorGraphPlate::cmModePolygon()
  {
  modeSet( new SdModeCPolygonEnter( this, getProjectItem() )  );
  }




void SdWEditorGraphPlate::cmModeDeleteWire()
  {
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

  //Activate tool bar
  SdWCommand::barPcb->show();
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




void SdWEditorGraphPlate::cmModePartSelect(QStringList list)
  {
  if( modeGet() )
    modeGet()->partSelect( list );
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





void SdWEditorGraphPlate::cmFileExport()
  {
  //Wizard
  QWizard wizard(this);
  //Fill it with pages
  SdPMasterList *master = new SdPMasterList( tr("Export plate"), tr("Select export master"), &wizard );
  wizard.setPage( 0,   master );
  wizard.setPage( 1,   new SdPExportPlate_Gerber( this, mPlate, 1, master, &wizard) );
  //wizard.setPage( SDP_NPI_MASTER, new SdPNewProjectItem_Master( &item, mProject, &wizard) );
  wizard.exec();
  }
