/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Context for graphics draw in 3d
*/
#include "SdContext3d.h"

SdContext3d::SdContext3d() :
  SdContext( SdPoint(2500,2500), nullptr )
  {

  }


void SdContext3d::setPen(int width, QColor color, int lineStyle)
  {
  }

void SdContext3d::line(SdPoint a, SdPoint b)
  {
  }

void SdContext3d::fillRect(SdRect r)
  {
  }

void SdContext3d::arc(SdPoint center, SdPoint start, SdPoint stop)
  {
  }

void SdContext3d::circle(SdPoint center, int radius)
  {
  }

void SdContext3d::circleFill(SdPoint center, int radius)
  {
  }

void SdContext3d::regionFill(const SdPointList &points, const SdPropLine &prop)
  {
  }

void SdContext3d::polygon(const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer)
  {
  }
