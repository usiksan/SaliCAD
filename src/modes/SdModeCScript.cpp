#include "SdModeCScript.h"
#include "objects/SdEnvir.h"
#include "objects/SdPropLine.h"
#include "objects/SdPropText.h"
#include "objects/SdGraphScript.h"
#include "windows/SdWCommand.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdWEditorGraph.h"
#include "windows/SdDExpressionEdit.h"

SdModeCScript::SdModeCScript(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdMode( editor, obj ),
  mPropText(nullptr),
  mEditScript(nullptr),
  mLinkItem(-1)
  {

  }




void SdModeCScript::activate()
  {
  SdMode::activate();
  }



void SdModeCScript::deactivate()
  {
  }

void SdModeCScript::reset()
  {
  }




void SdModeCScript::drawStatic(SdContext *ctx)
  {
  mObject->forEach( dctAll, [ctx]( SdObject *obj ) {
    SdPtr<SdGraph> graph(obj);
    if( graph.isValid() )
      graph->draw( ctx );
    return true;
    });
  }



void SdModeCScript::drawDynamic(SdContext *ctx)
  {
  if( mEditScript != nullptr ) {
    //Draw selection rectangle over linking item
    ctx->setPen(0,SdEnvir::instance()->getSysColor(scEnter), dltDotted );
    ctx->rect( mLinkOver );
    }
  }



int SdModeCScript::getPropBarId() const
  {
  return PB_TEXT;
  }



void SdModeCScript::propGetFromBar()
  {
  auto tbar = SdWCommand::getModeToolBar<SdPropBarTextual>( PB_TEXT );
  if( tbar ) {
    if( mPropText )
      tbar->getPropText( mPropText );
    else
      tbar->getPropText( &(sdGlobalProp->mTextProp) );
    mEditor->setFocus();
    update();
    }
  }



void SdModeCScript::propSetToBar()
  {
  auto tbar = SdWCommand::getModeToolBar<SdPropBarTextual>( PB_TEXT );
  if( tbar ) {
    if( mPropText )
      tbar->setPropText( mPropText, mEditor->getPPM() );
    else
      tbar->setPropText( &(sdGlobalProp->mTextProp), mEditor->getPPM() );
    }
  }



void SdModeCScript::enterPoint( SdPoint p )
  {
  if( getStep() == sPlace ) {
    //Here we insert new script or edit existing
    SdGraph *pgraph = nullptr;
    mObject->forEach( dctSymImp | dctGraphScript, [&pgraph,p]( SdObject *obj ) {
      SdPtr<SdGraph> graph(obj);
      if( graph.isValid() && graph->behindCursor(p) ) {
        pgraph = graph.ptr();
        return false;
        }
      return true;
      });

    if( pgraph == nullptr ) {
      //Mouse click in empty space, so we create new script
      SdDExpressionEdit edit( QString{}, mEditor );
      if( edit.exec() ) {
        //Create and insert new Script object
        addPic( new SdGraphScript( edit.scriptGet(), p, sdGlobalProp->mTextProp ), QStringLiteral("Append new script")  );
        }
      }
    else if( pgraph->getClass() == dctGraphScript ) {
      //Mouse click on existing script, so we edit it or link with other component
      SdPtr<SdGraphScript> script(pgraph);
      Q_ASSERT( script.isValid() );
      mLinkItem = script->tryLink( p, mLinkOver );
      if( mLinkItem >= 0 ) {
        //Will be link with other component
        mEditScript = script.ptr();
        setStep(sLink);
        update();
        }
      else {
        //Edit script
        SdDExpressionEdit edit( script->scriptGet(), mEditor );
        if( edit.exec() ) {
          //Update edited Script
          script->scriptSet( edit.scriptGet(), mUndo );
          setDirtyCashe();
          setDirty();
          update();
          }
        }
      }
    }
  else if( getStep() == sLink ) {
    //We find component to which will be linked currently editing script item
    SdGraphParam *pgraph = nullptr;
    mObject->forEach( dctSymImp | dctGraphScript, [&pgraph,p]( SdObject *obj ) {
      SdPtr<SdGraphParam> graph(obj);
      if( graph.isValid() && graph->behindCursor(p) ) {
        pgraph = graph.ptr();
        return false;
        }
      return true;
      });

    if( pgraph == nullptr ) {
      //Mouse click in empty space, delete currently existing linking
      mEditScript->link( mLinkItem, nullptr, nullptr, QString{} );
      }
    else {
      mEditScript->link( mLinkItem, mObject, pgraph, QString("value") );
      }
    mEditScript = nullptr;
    setStep( sPlace );
    setDirtyCashe();
    setDirty();
    update();
    }
  }



void SdModeCScript::clickPoint(SdPoint)
  {
  }



void SdModeCScript::cancelPoint(SdPoint)
  {
  cancelMode();
  }

void SdModeCScript::movePoint(SdPoint)
  {
  }

bool SdModeCScript::wheel(SdPoint p)
  {
  return SdMode::wheel(p);
  }

void SdModeCScript::keyDown(int key, QChar ch)
  {
  }

void SdModeCScript::keyUp(int key, QChar ch)
  {
  }

SdPoint SdModeCScript::enterPrev()
  {
  }

void SdModeCScript::beginDrag(SdPoint)
  {
  }

void SdModeCScript::dragPoint(SdPoint)
  {
  }

void SdModeCScript::stopDrag(SdPoint)
  {
  }



bool SdModeCScript::getInfo(SdPoint p, QString &info)
  {
  return false;
  }




QString SdModeCScript::getStepHelp() const
  {
  if( getStep() == sPlace ) return QObject::tr("Enter point to insert or edit script");
  if( getStep() == sLink ) return QObject::tr("Select component or other script to link with");
  if( getStep() == sOver ) return QObject::tr("Select group of components to find the right one calculation script");
  return QString();
  }




QString SdModeCScript::getModeThema() const
  {
  return QString( MODE_HELP "ModeCScript.htm" );
  }




QString SdModeCScript::getStepThema() const
  {
  if( getStep() == sPlace ) return QString( MODE_HELP "ModeCScript.htm#place" );
  if( getStep() == sLink ) return QString( MODE_HELP "ModeCScript.htm#link" );
  if( getStep() == sOver ) return QString( MODE_HELP "ModeCScript.htm#over" );
  return QString();
  }




int SdModeCScript::getCursor() const
  {
  return CUR_SCRIPT;
  }




int SdModeCScript::getIndex() const
  {
  return MD_SCRIPT;
  }
