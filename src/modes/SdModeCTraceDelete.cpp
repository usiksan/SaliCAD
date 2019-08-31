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

  }

void SdModeCTraceDelete::drawDynamic(SdContext *ctx)
  {

  }

void SdModeCTraceDelete::enterPoint(SdPoint)
  {

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

  }




int SdModeCTraceDelete::getIndex() const
  {
  return MD_TRACE_DELETE;
  }
