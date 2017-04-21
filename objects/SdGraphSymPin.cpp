#include "SdGraphSymPin.h"

SdGraphSymPin::SdGraphSymPin()
  {

  }


void SdGraphSymPin::attach(SdUndo *undo)
  {
  }

void SdGraphSymPin::undoAttach()
  {
  }

void SdGraphSymPin::detach(SdUndo *undo)
  {
  }

void SdGraphSymPin::undoDetach()
  {
  }

SdObject *SdGraphSymPin::copyNext()
  {
  }

void SdGraphSymPin::cloneFrom(const SdObject *src)
  {
  }

void SdGraphSymPin::writeObject(QJsonObject &obj) const
  {
  }

void SdGraphSymPin::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  }

void SdGraphSymPin::saveState(SdUndo *undo)
  {
  }

void SdGraphSymPin::move(SdPoint offset)
  {
  }

void SdGraphSymPin::rotate(SdPoint center, SdAngle angle)
  {
  }

void SdGraphSymPin::mirror(SdPoint a, SdPoint b)
  {
  }

void SdGraphSymPin::setProp(SdProp &prop)
  {
  }

void SdGraphSymPin::getProp(SdProp &prop)
  {
  }

void SdGraphSymPin::setText(int index, QString sour, SdPropText &prop)
  {
  }

void SdGraphSymPin::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  }

void SdGraphSymPin::selectByRect(const SdRect &r, SdSelector *selector)
  {
  }

void SdGraphSymPin::select(SdSelector *selector)
  {
  }

bool SdGraphSymPin::isAble()
  {
  }

SdRect SdGraphSymPin::getOverRect() const
  {
  }

void SdGraphSymPin::draw(SdContext *dc)
  {
  }

int SdGraphSymPin::behindCursor(SdPoint p)
  {
  }

int SdGraphSymPin::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {

  }

bool SdGraphSymPin::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  }

bool SdGraphSymPin::snapPoint(SdSnapInfo *snap)
  {
  }
