/*
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
#include "SdWCommand.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "modes/SdModeCSymPin.h"
#include "modes/SdModeCOrigin.h"
#include <QDebug>

SdWEditorGraphSymbol::SdWEditorGraphSymbol(SdPItemSymbol *sym, QWidget *parent) :
  SdWEditorGraph( sym, parent ),
  mSymbol(sym)
  {
  mGrid.set( 25, 25 );
  }



SdProjectItem *SdWEditorGraphSymbol::getProjectItem()
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


  //qDebug() << SdWCommand::barMain->iconSize();

  }


double SdWEditorGraphSymbol::getPPM() const
  {
  return sdEnvir->mSchPPM;
  }


void SdWEditorGraphSymbol::cmModePin()
  {
  modeSet( new SdModeCSymPin( this, mSymbol ) );
  }



void SdWEditorGraphSymbol::cmModeReference()
  {
  }



void SdWEditorGraphSymbol::cmModeOrigin()
  {
  modeSet( new SdModeCOrigin( this, mSymbol, sdEnvir->mSymPinSize ) );
  }
