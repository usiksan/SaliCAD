/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph editor for part of component
*/
#include "SdWEditorGraphPart.h"
#include "SdWCommand.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "modes/SdModeCOriginPart.h"
#include "modes/SdModeCPartPin.h"
#include <QDebug>

SdWEditorGraphPart::SdWEditorGraphPart(SdPItemPart *part, QWidget *parent) :
  SdWEditorGraph( part, parent ),
  mPart( part )
  {
  mGrid.set( 2500, 2500 );
  }


SdProjectItem *SdWEditorGraphPart::getProjectItem()
  {
  return mPart;
  }




void SdWEditorGraphPart::onActivateEditor()
  {
  SdWEditorGraph::onActivateEditor();

  //Activate menu
  SdWCommand::cmMenuInsertPart->setVisible(true);

  //Activate tool bar
  SdWCommand::barPart->show();
  }




void SdWEditorGraphPart::cmModePin()
  {
  modeSet( new SdModeCPartPin( this, mPart ) );
  }




void SdWEditorGraphPart::cmModeReference()
  {
  }




void SdWEditorGraphPart::cmModeOrigin()
  {
  modeSet( new SdModeCOriginPart( this, mPart, sdEnvir->mPrtPinSize ) );
  }




double SdWEditorGraphPart::getPPM() const
  {
  return sdEnvir->mPrtPPM;
  }
