﻿/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph editor for symbol precentation of component
*/

#include "SdWEditorGraphSymbol.h"
#include "SdDSymbolPartParam.h"
#include "SdWCommand.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "SdPMasterList.h"
#include "SdPExport_Dxf.h"
#include "modes/SdModeCSymPin.h"
#include "modes/SdModeCOrigin.h"
#include "modes/SdModeCIdent.h"
#include "modes/SdModeCValue.h"
#include "modes/SdModeSymbolFragment.h"
#include <QDebug>

SdWEditorGraphSymbol::SdWEditorGraphSymbol(SdPItemSymbol *sym, QWidget *parent) :
  SdWEditorGraph( sym, parent ),
  mSymbol(sym)
  {
  mGrid.set( 250, 250 );
  }





SdProjectItem *SdWEditorGraphSymbol::getProjectItem() const
  {
  return mSymbol;
  }


void SdWEditorGraphSymbol::onActivateEditor()
  {
  SdWEditorGraph::onActivateEditor();

  //Activate menu
  SdWCommand::cmMenuInsertSymbol->setVisible(true);

  //Activate tool bar
  SdWCommand::barSymbol->show();

  //Enable finish edit
  SdWCommand::cmObjectEditDisable->setVisible(true);

  //qDebug() << SdWCommand::barMain->iconSize();

  }




void SdWEditorGraphSymbol::cmModePin()
  {
  modeSet( new SdModeCSymPin( this, mSymbol ) );
  }



void SdWEditorGraphSymbol::cmModeReference()
  {
  modeSet( new SdModeCIdent( this, mSymbol, &(sdGlobalProp->mSymIdentProp), MD_SYM_IDENT )  );
  }



void SdWEditorGraphSymbol::cmModeOrigin()
  {
  modeSet( new SdModeCOrigin( this, mSymbol, sdEnvir->mSymPinSize ) );
  }




void SdWEditorGraphSymbol::cmModeValue()
  {
  modeSet( new SdModeCValue( this, mSymbol, &(sdGlobalProp->mSymValueProp), MD_SYM_VALUE )  );
  }




void SdWEditorGraphSymbol::cmModeSymbolFragment()
  {
  modeSet( new SdModeSymbolFragment( this, mSymbol ) );
  }




void SdWEditorGraphSymbol::cmSymbolPartParam()
  {
  SdDSymbolPartParam editor( mSymbol, this );
  editor.exec();
  }





//Export command
void SdWEditorGraphSymbol::cmFileExport()
  {
  //Wizard
  QWizard wizard(this);
  //Fill it with pages
  SdPMasterList *master = new SdPMasterList( tr("Export symbol"), tr("Select export master"), &wizard );
  wizard.setPage( 0,   master );
  wizard.setPage( 1,   new SdPExport_Dxf( mSymbol, 1, master, &wizard) );
  wizard.exec();
  }



//Disable show field contents
bool SdWEditorGraphSymbol::showFields()
  {
  return true;
  }
