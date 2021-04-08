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
  mToDelete(nullptr)
  {

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
  ctx->setOverColor( sdEnvir->getSysColor(scSelected) );
  mFragment.draw( ctx );
  }



void SdModeCTraceDelete::enterPoint(SdPoint)
  {
  if( mFragment.count() ) {
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

  }



void SdModeCTraceDelete::cancelPoint(SdPoint)
  {

  }



void SdModeCTraceDelete::beginDrag(SdPoint p)
  {

  }

void SdModeCTraceDelete::dragPoint(SdPoint p)
  {

  }

void SdModeCTraceDelete::stopDrag(SdPoint p)
  {

  }

QString SdModeCTraceDelete::getStepHelp() const
  {

  }

QString SdModeCTraceDelete::getModeThema() const
  {

  }

QString SdModeCTraceDelete::getStepThema() const
  {

  }



int SdModeCTraceDelete::getCursor() const
  {
  return CUR_CUT;
  }




int SdModeCTraceDelete::getIndex() const
  {
  return MD_TRACE_DELETE;
  }
