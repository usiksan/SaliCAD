/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for delete plate trace road

  When moving cursor we find nearest point to delete segment. This point is road-middle or road end.

  When press left button we delete segment.

*/
#include "SdModeCTraceDelete.h"
#include "objects/SdSnapInfo.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphTracedRoad.h"
#include "objects/SdGraphTracedVia.h"
#include "windows/SdWEditorGraph.h"

SdModeCTraceDelete::SdModeCTraceDelete(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mShift(false)
  {

  }


SdModeCTraceDelete::~SdModeCTraceDelete()
  {
  mFragment.removeAll();
  }


void SdModeCTraceDelete::deactivate()
  {
  mFragment.removeAll();
  SdModeCommon::deactivate();
  }


void SdModeCTraceDelete::reset()
  {
  mFragment.removeAll();
  SdModeCommon::reset();
  }




void SdModeCTraceDelete::drawStatic(SdContext *ctx)
  {
  //Draw all object except selected
  mObject->forEach( dctAll, [this, ctx] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph != nullptr && graph->getSelector() != &mFragment )
      graph->draw( ctx );
    return true;
    });
  }




void SdModeCTraceDelete::drawDynamic(SdContext *ctx)
  {
  //Draw all selected elements
  ctx->setOverColor( SdEnvir::instance()->getSysColor(scSelected) );
  mFragment.draw( ctx );
  if( getStep() == 1 ) {
    ctx->setPen( 0, SdEnvir::instance()->getSysColor(scEnter), dltDotted );
    ctx->rect( SdRect(mFirst, mPrev) );
    }
  }



void SdModeCTraceDelete::enterPoint(SdPoint)
  {
  if( mFragment.count() && !mShift ) {
    mUndo->begin( QObject::tr("Deletion elements"), mObject, false );
    mFragment.forEach( dctAll, [this] (SdObject *obj) ->bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr )
        graph->deleteObject( mUndo );
      return true;
      });
    mFragment.removeAll();

    setDirty();
    setDirtyCashe();
    setDirtyRatNet();
    update();
    }
  }





void SdModeCTraceDelete::movePoint(SdPoint p)
  {
  if( mFragment.count() )
    mFragment.removeAll();
  if( !mShift ) {
    //Find nearest smart point
    SdSnapInfo info;
    info.mSour = p;
    info.mSnapMask = snapNearestNet | snapEndPoint | snapMidPoint | snapViaPoint;
    info.mStratum = stmThrough;
    info.scan( plate(), dctTraceVia | dctTraceRoad | dctTracePolygon );
    if( info.mGraph != nullptr )
      info.mGraph->select( &mFragment );
    }
  mPrev = p;
  setDirtyCashe();
  update();
  }




void SdModeCTraceDelete::cancelPoint(SdPoint)
  {
  cancelMode();
  }




void SdModeCTraceDelete::beginDrag(SdPoint point)
  {
  setStep(1);
  mFirst = point;
  mPrev = point;
  update();
  }



void SdModeCTraceDelete::dragPoint(SdPoint p)
  {
  mFragment.removeAll();
  if( mShift ) {
    //Select all trace behind over rectangle
    SdRect r(mFirst,p);
    mObject->forEach( dctTraceVia | dctTraceRoad | dctTracePolygon, [this,r] (SdObject *obj) ->bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr )
        graph->selectByRect( r, &mFragment );
      return true;
      });
    }
  mPrev = p;
  setDirtyCashe();
  update();
  }




void SdModeCTraceDelete::stopDrag(SdPoint p)
  {
  setStep(0);
  if( mShift ) {
    mShift = false;
    enterPoint(p);
    mShift = true;
    }
  }


QString SdModeCTraceDelete::getStepHelp() const
  {
  return QObject::tr("Click trace object to delete");
  }



QString SdModeCTraceDelete::getModeThema() const
  {
  return QString( MODE_HELP "ModeCTraceDelete.htm" );
  }



QString SdModeCTraceDelete::getStepThema() const
  {
  return QString( MODE_HELP "ModeCTraceDelete.htm" );
  }



int SdModeCTraceDelete::getCursor() const
  {
  return CUR_CUT;
  }




int SdModeCTraceDelete::getIndex() const
  {
  return MD_TRACE_DELETE;
  }



void SdModeCTraceDelete::keyDown(int key, QChar ch)
  {
  Q_UNUSED(ch)
  if( key == Qt::Key_Shift ) {
    mShift = true;
    if( getStep() == 0 ) movePoint( mPrev );
    else                 dragPoint( mPrev );
    }
  }



void SdModeCTraceDelete::keyUp(int key, QChar ch)
  {
  Q_UNUSED(ch)
  if( key == Qt::Key_Shift ) {
    mShift = false;
    if( getStep() == 0 ) movePoint( mPrev );
    else                 dragPoint( mPrev );
    }
  }
