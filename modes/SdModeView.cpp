/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for view graph object.
  All other mode except this, if set - enter edit mode of graph object.
  This mode NOT enter edit mode, so it may be used in simple view of object.
*/
#include "SdModeView.h"
#include "windows/SdWEditorGraph.h"
#include <QObject>

SdModeView::SdModeView(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdMode( editor, obj )
  {

  }




void SdModeView::drawStatic(SdContext *ctx)
  {
  mObject->draw( ctx );
  }




void SdModeView::beginDrag( SdPoint p )
  {
  mBeginDrag = p;
  setStep(1);
  }




void SdModeView::dragPoint( SdPoint p )
  {
  p = p - mBeginDrag;
  SdPoint org = mEditor->originGet();
  org += p;
  mEditor->originSet( org );
  mBeginDrag += p;
  }




void SdModeView::stopDrag(SdPoint)
  {
  setStep(0);
  }







QString SdModeView::getStepHelp() const
  {
  return getStep() == 0 ? QObject::tr("Select point to pan view") : QObject::tr("Drag to pan view");
  }




QString SdModeView::getModeThema() const
  {
  return QString( MODE_HELP "ModeView.htm" );
  }




QString SdModeView::getStepThema() const
  {
  return getStep() == 0 ? QString( MODE_HELP "ModeView.htm#beginDrag" ) : QString( MODE_HELP "ModeView.htm#dragging" );
  }




int SdModeView::getCursor() const
  {
  return CUR_ARROW;
  }




int SdModeView::getIndex() const
  {
  return MD_VIEW;
  }

