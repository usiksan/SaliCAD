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
#include "modes/SdModeCIdent.h"
#include <QDebug>

SdWEditorGraphSymbol::SdWEditorGraphSymbol(SdPItemSymbol *sym, QWidget *parent) :
  SdWEditorGraph( sym, parent ),
  mSymbol(sym)
  {
  mGrid.set( 25, 25 );
  }




void SdWEditorGraphSymbol::setSymbol(SdPItemSymbol *sym)
  {
  //Delete previous symbol
  if( mSymbol ) {
    delete mSymbol;
    mSymbol = 0;
    }

  //Set new symbol
  mSymbol = sym;

  if( mSymbol ) {
    //Fill symbol in view
    cmViewFit();
    }
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
