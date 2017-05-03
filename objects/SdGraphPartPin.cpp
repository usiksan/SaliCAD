/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Part pin.
*/
#include "SdGraphPartPin.h"

SdGraphPartPin::SdGraphPartPin()
  {

  }

void SdGraphPartPin::attach(SdUndo *undo)
  {

  }

void SdGraphPartPin::undoAttach()
  {

  }

void SdGraphPartPin::detach(SdUndo *undo)
  {

  }

void SdGraphPartPin::undoDetach()
  {

  }

SdObject *SdGraphPartPin::copyNext()
  {

  }

void SdGraphPartPin::cloneFrom(const SdObject *src)
  {

  }

void SdGraphPartPin::writeObject(QJsonObject &obj) const
  {

  }

void SdGraphPartPin::readObject(SdObjectMap *map, const QJsonObject obj)
  {

  }

void SdGraphPartPin::saveState(SdUndo *undo)
  {

  }

void SdGraphPartPin::move(SdPoint offset)
  {

  }

void SdGraphPartPin::rotate(SdPoint center, SdAngle angle)
  {

  }

void SdGraphPartPin::mirror(SdPoint a, SdPoint b)
  {

  }

void SdGraphPartPin::setProp(SdProp &prop)
  {

  }

void SdGraphPartPin::getProp(SdProp &prop)
  {

  }

void SdGraphPartPin::setText(int index, QString sour, SdPropText &prop)
  {

  }

void SdGraphPartPin::selectByPoint(const SdPoint p, SdSelector *selector)
  {

  }

void SdGraphPartPin::selectByRect(const SdRect &r, SdSelector *selector)
  {

  }

void SdGraphPartPin::select(SdSelector *selector)
  {

  }

bool SdGraphPartPin::isVisible()
  {

  }

SdRect SdGraphPartPin::getOverRect() const
  {

  }

void SdGraphPartPin::draw(SdContext *dc)
  {

  }

int SdGraphPartPin::behindCursor(SdPoint p)
  {

  }

int SdGraphPartPin::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {

  }

bool SdGraphPartPin::getInfo(SdPoint p, QString &info, bool extInfo)
  {

  }

bool SdGraphPartPin::snapPoint(SdSnapInfo *snap)
  {

  }
