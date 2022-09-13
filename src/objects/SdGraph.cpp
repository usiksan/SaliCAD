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
  mSelector(nullptr)
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

void SdGraph::rotate(SdPoint center, SdPropAngle angle)
  {
  Q_UNUSED(center)
  Q_UNUSED(angle)
  }

void SdGraph::mirror(SdPoint a, SdPoint b)
  {
  Q_UNUSED(a)
  Q_UNUSED(b)
  }

void SdGraph::setProp(SdPropSelected &prop)
  {
  Q_UNUSED(prop)
  }

void SdGraph::getProp(SdPropSelected &prop)
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
  if( selector != nullptr )
    selector->insert( this );
  }



void SdGraph::prepareMove(SdUndo *undo)
  {
  Q_UNUSED(undo)
  }






void SdGraph::setLayerUsage()
  {
  }



void SdGraph::draw(SdContext *dc)
  {
  Q_UNUSED(dc)
  }



//!
//! \brief drawExceptText Draws object in 2d space with exception textual component with textId
//! \param dc             Display context where drawing doing
//! \param textId         Text id which must not be drawn
//!
void SdGraph::drawExceptText(SdContext *dc, int textId)
  {
  Q_UNUSED(dc)
  Q_UNUSED(textId)
  }




void SdGraph::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  Q_UNUSED(f)
  }




//!
//! \brief accumHoles Accum holes description into faceList
//! \param model      Model which accumulate coord vertexes
//! \param faceList   Face list for holding holes (single face for single hole)
//! \param stratum    Stratum for layers
//! \param map        Map for holes conversion
//!
void SdGraph::accumHoles(Sd3drModel &model, Sd3drFaceList &faceList, SdStratum stratum, const QMatrix4x4 &map) const
  {
  Q_UNUSED(model)
  Q_UNUSED(faceList)
  Q_UNUSED(stratum)
  Q_UNUSED(map)
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



//Find snap point on object
void SdGraph::snapPoint(SdSnapInfo *snap)
  {
  Q_UNUSED(snap)
  }



