/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base for objects with graphical presentation
*/

#include "SdGraph.h"
#include "SdSelector.h"

SdGraph::SdGraph() :
  SdObject(),
  mSelector(0)
  {

  }

void SdGraph::saveState(SdUndo *undo)
  {
  Q_UNUSED(undo)
  }




void SdGraph::moveComplete(SdPoint grid, SdUndo *undo)
  {
  Q_UNUSED(undo)
  Q_UNUSED(grid)
  }




void SdGraph::move(SdPoint offset)
  {
  Q_UNUSED(offset)
  }

void SdGraph::rotate(SdPoint center, SdAngle angle)
  {
  Q_UNUSED(center)
  Q_UNUSED(angle)
  }

void SdGraph::mirror(SdPoint a, SdPoint b)
  {
  Q_UNUSED(a)
  Q_UNUSED(b)
  }

void SdGraph::setProp(SdProp &prop)
  {
  Q_UNUSED(prop)
  }

void SdGraph::getProp(SdProp &prop)
  {
  Q_UNUSED(prop)
  }

void SdGraph::setText(int index, QString sour, SdPropText &prop, QWidget *parent)
  {
  Q_UNUSED(index)
  Q_UNUSED(sour)
  Q_UNUSED(prop)
  Q_UNUSED(parent)
  }

void SdGraph::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  Q_UNUSED(p)
  Q_UNUSED(selector)
  }

void SdGraph::selectByRect(const SdRect &r, SdSelector *selector)
  {
  Q_UNUSED(r)
  Q_UNUSED(selector)
  }

void SdGraph::select(SdSelector *selector)
  {
  selector->insert( this );
  }



void SdGraph::prepareMove()
  {

  }




bool SdGraph::canHideLayer(SdLayer *layer)
  {
  Q_UNUSED(layer)
  return true;
  }



void SdGraph::draw(SdContext *dc)
  {
  Q_UNUSED(dc)
  }



int SdGraph::behindCursor(SdPoint p)
  {
  Q_UNUSED(p)
  return 0;
  }



int SdGraph::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {
  Q_UNUSED(p)
  Q_UNUSED(org)
  Q_UNUSED(dest)
  Q_UNUSED(prop)
  return 0;
  }

bool SdGraph::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  Q_UNUSED(p)
  Q_UNUSED(info)
  Q_UNUSED(extInfo)
  return false;
  }

bool SdGraph::snapPoint(SdSnapInfo *snap)
  {
  Q_UNUSED(snap)
  return false;
  }

