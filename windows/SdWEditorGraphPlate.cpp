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




SdProjectItem *SdWEditorGraphPlate::getProjectItem()
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

double SdWEditorGraphPlate::getPPM() const
  {
  return sdEnvir->mPrtPPM;
  }


void SdWEditorGraphPlate::onActivateEditor()
  {
  SdWEditorGraph::onActivateEditor();

  //Activate menu
  SdWCommand::cmMenuInsertPcb->setVisible(true);

  //Activate tool bar
  SdWCommand::barPcb->show();
  }
