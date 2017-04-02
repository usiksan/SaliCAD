/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdGraphObject.h"

SdGraphObject::SdGraphObject()
  {

  }

void SdGraphObject::doSelect(SdSelector *selector)
  {

  }

void SdGraphObject::move(SdPoint offset)
  {
  Q_UNUSED(offset)
  }

void SdGraphObject::rotate(SdPoint center, SdAngle angle)
  {
  Q_UNUSED(center)
  Q_UNUSED(angle)
  }

void SdGraphObject::mirror(SdPoint a, SdPoint b)
  {
  Q_UNUSED(a)
  Q_UNUSED(b)
  }

void SdGraphObject::setProp(SdProp &prop)
  {
  Q_UNUSED(prop)
  }

void SdGraphObject::getProp(SdProp &prop)
  {
  Q_UNUSED(prop)
  }

void SdGraphObject::setText(int index, QString sour, SdTextProp &prop)
  {
  Q_UNUSED(index)
  Q_UNUSED(sour)
  Q_UNUSED(prop)
  }

void SdGraphObject::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  Q_UNUSED(p)
  Q_UNUSED(selector)
  }

void SdGraphObject::selectByRect(const SdRect &r, SdSelector *selector)
  {
  Q_UNUSED(r)
  Q_UNUSED(selector)
  }

void SdGraphObject::select(SdSelector *selector)
  {
  doSelect( selector );
  }

bool SdGraphObject::canDelLayer(int layer)
  {
  Q_UNUSED(layer)
  return true;
  }

bool SdGraphObject::canDelFont(int layer)
  {
  Q_UNUSED(layer)
  return true;
  }

void SdGraphObject::draw(SdContext *dc)
  {
  Q_UNUSED(dc)
  }

int SdGraphObject::behindCursor(SdPoint p)
  {
  Q_UNUSED(p)
  return 0;
  }

int SdGraphObject::behindText(SdPoint p, QString &dest, SdTextProp &prop)
  {
  Q_UNUSED(p)
  Q_UNUSED(dest)
  Q_UNUSED(prop)
  return 0;
  }

bool SdGraphObject::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  Q_UNUSED(p)
  Q_UNUSED(info)
  Q_UNUSED(extInfo)
  return false;
  }

bool SdGraphObject::snapPoint(SdSnapInfo &snap)
  {
  Q_UNUSED(snap)
  return false;
  }

