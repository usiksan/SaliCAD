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
  mIdent    = obj->getIdent();
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
  if( getStep() ) {
    drawText( ctx );
    }
  else {
    SdRect r;
    ctx->text( mPrev, r, mString, *mPropText );
    }
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
  }




void SdModeCIdent::propSetToBar()
  {
  SdPropBarTextual *bar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
  Q_ASSERT( bar != nullptr );
  bar->setPropText( mPropText, mEditor->getPPM() );
  }



void SdModeCIdent::enterPoint(SdPoint)
  {
  if( getStep() ) applyEdit();
  else {
    setStep(1);
    update();
    }
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
  if( getStep() ) return QObject::tr("Edit ident prefix");
  return QObject::tr("Enter ident position");
  }




QString SdModeCIdent::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCIdent.htm" );
  }





QString SdModeCIdent::getStepThema() const
  {
  if( getStep() ) return QStringLiteral( MODE_HELP "ModeCIdent.htm#editPrefix" );
  return QStringLiteral( MODE_HELP "ModeCIdent.htm#identPosition" );
  }




int SdModeCIdent::getIndex() const
  {
  return mIndex;
  }




void SdModeCIdent::cancelEdit()
  {
  setStep(0);
  }



void SdModeCIdent::applyEdit()
  {
  //Save previous state of ident
  mUndo->begin( QObject::tr("Edit ident") );
  mIdent->saveState( mUndo );
  mIdent->updateIdent( mPrev, mString, mOverRect, mPropText );
  setDirty();
  setDirtyCashe();
  cancelMode();
  }


void SdModeCIdent::activate()
  {
  //Make all text selected
  setText( mIdent->getText(), true );
  }


int SdModeCIdent::getCursor() const
  {
  return getStep() == 0 ? CUR_IDENT : CUR_TEXT;
  }
