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
#include "modes/SdModeCNetWire.h"
#include "modes/SdModeCLinearArea.h"
#include "modes/SdModeCNetName.h"
#include "modes/SdModeWireDisconnect.h"
#include "modes/SdModeCBus.h"
#include "modes/SdModeCFragment.h"
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



void SdWEditorGraphSheet::cmModeNetList()
  {
  }



