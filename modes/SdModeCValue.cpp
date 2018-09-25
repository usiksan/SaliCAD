/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for change position and properties of value
*/
#include "SdModeCValue.h"
#include "objects/SdGraph.h"
#include "objects/SdGraphValue.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"
#include <QObject>

SdModeCValue::SdModeCValue(SdWEditorGraph *editor, SdProjectItem *obj, SdPropText *prp, int index) :
  SdModeCTextual( editor, obj )
  {
  mValue    = obj->valueGet();
  mString   = mValue->getText();
  mPropText = prp;
  mIndex    = index;
  }





void SdModeCValue::activate()
  {
  SdModeCTextual::activate();
  //Make all text selected
  setText( mValue->getText(), true );
  }




void SdModeCValue::drawStatic(SdContext *ctx)
  {
  //Draw all element except value
  mObject->forEach( (dctAll & ~dctValue), [ctx] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph )
      graph->draw( ctx );
    return true;
    });
  }




void SdModeCValue::drawDynamic(SdContext *ctx)
  {
  SdRect r;
  ctx->text( mPrev, r, mString, *mPropText );
  }




int SdModeCValue::getPropBarId() const
  {
  return PB_TEXT;
  }






void SdModeCValue::propGetFromBar()
  {
  SdPropBarTextual *bar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
  Q_ASSERT( bar != nullptr );
  bar->getPropText( mPropText );
  update();
  }






void SdModeCValue::propSetToBar()
  {
  SdPropBarTextual *bar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
  Q_ASSERT( bar != nullptr );
  bar->setPropText( mPropText, mEditor->getPPM() );
  }




void SdModeCValue::enterPoint(SdPoint)
  {
  //Save previous state of ident
  mUndo->begin( QObject::tr("Edit value"), mObject );
  mValue->saveState( mUndo );
  mValue->updateValue( mPrev, mOverRect, mPropText );
  setDirty();
  setDirtyCashe();
  cancelMode();
  }




void SdModeCValue::cancelPoint(SdPoint)
  {
  cancelMode();
  }





void SdModeCValue::movePoint(SdPoint p)
  {
  if( getStep() == 0 ) {
    mPrev = p;
    update();
    }
  }





QString SdModeCValue::getStepHelp() const
  {
  return QObject::tr("Enter value position");
  }




QString SdModeCValue::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCValue.htm" );
  }




QString SdModeCValue::getStepThema() const
  {
  return getModeThema();
  }




int SdModeCValue::getCursor() const
  {
  return CUR_IDENT;
  }




int SdModeCValue::getIndex() const
  {
  return mIndex;
  }




