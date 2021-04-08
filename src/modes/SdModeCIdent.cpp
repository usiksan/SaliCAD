/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for change position and properties of ident
*/
#include "SdModeCIdent.h"
#include "objects/SdGraph.h"
#include "objects/SdGraphIdent.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"
#include <QObject>
#include <QDebug>

SdModeCIdent::SdModeCIdent(SdWEditorGraph *editor, SdProjectItem *obj, SdPropText *prp , int index) :
  SdModeCTextual( editor, obj )
  {
  mIdent    = obj->identGet();
  mString   = mIdent->getText();
  mPropText = prp;
  mIndex    = index;
  }



void SdModeCIdent::drawStatic(SdContext *ctx)
  {
  //Draw all element except ident
  mObject->forEach( (dctAll & ~dctIdent), [ctx] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph )
      graph->draw( ctx );
    return true;
    });
  }



void SdModeCIdent::drawDynamic(SdContext *ctx)
  {
  SdRect r;
  ctx->text( mPrev, r, mString, *mPropText );
  }



int SdModeCIdent::getPropBarId() const
  {
  return PB_TEXT;
  }




void SdModeCIdent::propGetFromBar()
  {
  SdPropBarTextual *bar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
  Q_ASSERT( bar != nullptr );
  bar->getPropText( mPropText );
  update();
  }




void SdModeCIdent::propSetToBar()
  {
  SdPropBarTextual *bar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
  Q_ASSERT( bar != nullptr );
  bar->setPropText( mPropText, mEditor->getPPM() );
  }



void SdModeCIdent::enterPoint(SdPoint)
  {
  //Save previous state of ident
  mUndo->begin( QObject::tr("Edit ident"), mObject, false );
  mIdent->saveState( mUndo );
  mIdent->updateIdent( mPrev, mOverRect, mPropText );
  setDirty();
  setDirtyCashe();
  cancelMode();
  }




void SdModeCIdent::cancelPoint(SdPoint)
  {
  cancelMode();
  }



void SdModeCIdent::movePoint(SdPoint p)
  {
  if( getStep() == 0 ) {
    mPrev = p;
    update();
    }
  }



QString SdModeCIdent::getStepHelp() const
  {
  return QObject::tr("Enter ident position");
  }




QString SdModeCIdent::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCIdent.htm" );
  }





QString SdModeCIdent::getStepThema() const
  {
  return getModeThema();
  }




int SdModeCIdent::getIndex() const
  {
  return mIndex;
  }




void SdModeCIdent::cancelEdit()
  {
  setStep(0);
  }





void SdModeCIdent::activate()
  {
  SdModeCTextual::activate();
  //Make all text selected
  setText( mIdent->getText(), true );
  }



int SdModeCIdent::getCursor() const
  {
  return CUR_IDENT;
  }
