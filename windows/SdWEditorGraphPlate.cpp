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
#include <QDebug>

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

void SdWEditorGraphPlate::cmModeWire()
  {
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
    mPlate->setPadAssociation( pads.getAssociationName(), pads.getPadMap(), mPlate->getUndo() );
    }
  }
