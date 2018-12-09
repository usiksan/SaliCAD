/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for move plate trace road
*/
#include "SdModeCRoadMove.h"

SdModeCRoadMove::SdModeCRoadMove(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {

  }



void SdModeCRoadMove::reset()
  {
  setDirtyCashe();
  setStep( sFindRoad );
  }




void SdModeCRoadMove::drawStatic(SdContext *ctx)
  {
  plate()->drawTrace( ctx, mProp.mStratum, QString() );
  }




void SdModeCRoadMove::drawDynamic(SdContext *ctx)
  {
  }




void SdModeCRoadMove::movePoint(SdPoint)
  {
  }




void SdModeCRoadMove::beginDrag(SdPoint)
  {
  }




void SdModeCRoadMove::dragPoint(SdPoint)
  {
  }




void SdModeCRoadMove::stopDrag(SdPoint)
  {
  }




QString SdModeCRoadMove::getStepHelp() const
  {
  return getStep() == sMoveRoad ? QObject::tr("Move road to new position") : QObject::tr("Select road segment to move");
  }




QString SdModeCRoadMove::getModeThema() const
  {
  return  QString( MODE_HELP "ModeCRoadMove.htm" );
  }




QString SdModeCRoadMove::getStepThema() const
  {
  return getStep() == sMoveRoad ? QString( MODE_HELP "ModeCRoadMove.htm#moveRoad" ) : QString( MODE_HELP "ModeCRoadMove.htm#selectRoad" );
  }







int SdModeCRoadMove::getCursor() const
  {
  return CUR_WMOVE;
  }




int SdModeCRoadMove::getIndex() const
  {
  return MD_ROAD_MOVE;
  }
