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
#include "modes/SdModeCSymImp.h"
#include "modes/SdModeCWire.h"
#include "modes/SdModeCLinearArea.h"
#include "modes/SdModeCWireName.h"
#include "modes/SdModeWireDisconnect.h"
#include "modes/SdModeCBus.h"
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


//Component insertion mode
void SdWEditorGraphSheet::cmModeComponent()
  {
  modeSet( new SdModeCSymImp( this, mSheet ) );
  }



//Sheet net wire insertion mode
void SdWEditorGraphSheet::cmModeNet()
  {
  modeSet( new SdModeCWire( this, mSheet ) );
  }




void SdWEditorGraphSheet::cmModeBus()
  {
  modeSet( new SdModeCBus( this, mSheet )  );
  }




void SdWEditorGraphSheet::cmModeDisconnect()
  {
  modeSet( new SdModeWireDisconnect( this, mSheet )  );
  }



void SdWEditorGraphSheet::cmModePcbArea()
  {
  modeSet( new SdModeCLinearArea( this, mSheet ) );
  }



void SdWEditorGraphSheet::cmModeNetName()
  {
  modeSet( new SdModeCWireName( this, mSheet ) );
  }



void SdWEditorGraphSheet::cmModeNetList()
  {
  }



double SdWEditorGraphSheet::getPPM() const
  {
  return sdEnvir->mSchPPM;
  }
