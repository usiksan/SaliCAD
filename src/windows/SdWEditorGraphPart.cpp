﻿/*
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
#include "SdPMasterList.h"
#include "SdPExport_Dxf.h"
#include "modes/SdModeCOriginPart.h"
#include "modes/SdModeCPartPin.h"
#include "modes/SdModeCIdent.h"
#include "modes/SdModeCValue.h"
#include <QDebug>

SdWEditorGraphPart::SdWEditorGraphPart(SdPItemPart *part, QWidget *parent) :
  SdWEditorGraph( part, parent ),
  mPart( part )
  {
  mGrid.set( 2500, 2500 );
  mScale.scaleSet( 0.01 );
  }



void SdWEditorGraphPart::setPart(SdPItemPart *part)
  {
  //Delete previous part
  if( mPart ) {
    delete mPart;
    mPart = nullptr;
    }

  //Set new part
  mPart = part;

  if( mPart ) {
    //Fill part in view
    cmViewFit();
    }
  }



SdProjectItem *SdWEditorGraphPart::getProjectItem() const
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

  //Enable finish edit
  SdWCommand::cmObjectEditDisable->setVisible(true);
  }




void SdWEditorGraphPart::cmModePin()
  {
  modeSet( new SdModeCPartPin( this, mPart ) );
  }




void SdWEditorGraphPart::cmModeReference()
  {
  modeSet( new SdModeCIdent( this, mPart, &(sdGlobalProp->mPartIdentProp), MD_PART_IDENT )  );
  }




void SdWEditorGraphPart::cmModeOrigin()
  {
  modeSet( new SdModeCOriginPart( this, mPart, sdEnvir->mPartPinSize ) );
  }




void SdWEditorGraphPart::cmModeValue()
  {
  modeSet( new SdModeCValue( this, mPart, &(sdGlobalProp->mPartValueProp), MD_PART_VALUE )  );
  }




//Export command
void SdWEditorGraphPart::cmFileExport()
  {
  //Wizard
  QWizard wizard(this);
  //Fill it with pages
  SdPMasterList *master = new SdPMasterList( tr("Export part"), tr("Select export master"), &wizard );
  wizard.setPage( 0,   master );
  wizard.setPage( 1,   new SdPExport_Dxf( mPart, 1, master, &wizard) );
  wizard.exec();
  }





//Disble show fields contents
bool SdWEditorGraphPart::showFields()
  {
  return true;
  }
