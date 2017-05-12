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
#include "SdWCommand.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include <QDebug>

SdWEditorGraphSheet::SdWEditorGraphSheet(SdPItemSheet *sch, QWidget *parent) :
  SdWEditorGraph( sch, parent ),
  mSheet(sch)
  {
  mGrid.set( 25, 25 );
  }




SdProjectItem *SdWEditorGraphSheet::getProjectItem()
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

void SdWEditorGraphSheet::cmModeComponent()
  {
  }

void SdWEditorGraphSheet::cmModeNet()
  {
  }

void SdWEditorGraphSheet::cmModeBus()
  {
  }

void SdWEditorGraphSheet::cmModeDisconnect()
  {
  }

void SdWEditorGraphSheet::cmModePcbArea()
  {
  }

void SdWEditorGraphSheet::cmModeNetName()
  {
  }

void SdWEditorGraphSheet::cmModeNetList()
  {
  }

double SdWEditorGraphSheet::getPPM() const
  {
  return sdEnvir->mSchPPM;
  }
