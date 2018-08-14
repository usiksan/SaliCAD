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
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "modes/SdModeCRoadEnter.h"
#include <QDebug>
#include <QProgressDialog>

SdWEditorGraphPlate::SdWEditorGraphPlate(SdPItemPlate *pcb, QWidget *parent) :
  SdWEditorGraph( pcb, parent ),
  mPlate(pcb)
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




void SdWEditorGraphPlate::cmCheckRules()
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
