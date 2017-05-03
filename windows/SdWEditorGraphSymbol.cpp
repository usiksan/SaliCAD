/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdWEditorGraphSymbol.h"
#include "SdWCommand.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
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
  }

void SdWEditorGraphSymbol::cmModeReference()
  {
  }

void SdWEditorGraphSymbol::cmModeOrigin()
  {
  }
