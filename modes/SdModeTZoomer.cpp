/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Temporary mode.
  Zooming.
  Support two modes: zoomIn and zoomOut, selected on construct
*/
#include "SdModeTZoomer.h"
#include "windows/SdWEditorGraph.h"
#include <QObject>


SdModeTZoomer::SdModeTZoomer(bool zoomIn, SdWEditorGraph *editor, SdProjectItem *obj ) :
  SdModeTemp ( editor, obj ),
  mZoomIn(zoomIn)
  {

  }



void SdModeTZoomer::enterPoint(SdPoint point)
  {
  if( mZoomIn ) mEditor->scaleStep( 2.0 );
  else          mEditor->scaleStep( 0.5 );

  mEditor->originSet(point);
  }



void SdModeTZoomer::cancelPoint(SdPoint point)
  {
  Q_UNUSED(point)
  cancelMode();
  }

QString SdModeTZoomer::getStepHelp() const
  {
  //return zoomIn ? "Нажмите мышью для увеличения" : "Нажмите мышью для уменьшения";
  return mZoomIn ? QObject::tr( "Press mouse left button to zoom-in" ) :
                   QObject::tr( "Press mouse left button to zoom-out" );
  }

QString SdModeTZoomer::getModeHelp() const
  {
  return getStepThema();
  }

QString SdModeTZoomer::getStepThema() const
  {
  return mZoomIn ? QString( MODE_HELP "ModeZoomIn" ) : QString( MODE_HELP "ModeZoomOut" );
  }

int SdModeTZoomer::getCursor() const
  {
  return mZoomIn ? CUR_ZOOM_IN : CUR_ZOOM_OUT;
  }

int SdModeTZoomer::getIndex() const
  {
  return mZoomIn ? MD_ZOOM_IN : MD_ZOOM_OUT;
  }

