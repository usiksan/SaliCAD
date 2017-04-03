/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdWEditorGraph.h"
#include "objects/SdContext.h"
#include <QPainter>

SdWEditorGraph::SdWEditorGraph(QWidget *parent) :
  SdWEditor( parent )
  {

  }




void SdWEditorGraph::paintEvent(QPaintEvent *event)
  {
  QPainter painter( viewport() );

  }
