/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base class for all 3d view modes
*/
#include "Sd3dMode.h"

Sd3dMode::Sd3dMode()
  {

  }

void Sd3dMode::mousePressEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  Q_UNUSED(viewer)
  Q_UNUSED(event)
  }



void Sd3dMode::mouseReleaseEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  Q_UNUSED(viewer)
  Q_UNUSED(event)
  }



bool Sd3dMode::mouseMoveEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  Q_UNUSED(viewer)
  Q_UNUSED(event)
  return false;
  }



bool Sd3dMode::wheelEvent(SdWView3d *viewer, QWheelEvent *event)
  {
  Q_UNUSED(viewer)
  Q_UNUSED(event)
  return false;
  }



void Sd3dMode::keyPressEvent(SdWView3d *viewer, QKeyEvent *event)
  {
  Q_UNUSED(viewer)
  Q_UNUSED(event)
  }



void Sd3dMode::keyReleaseEvent(SdWView3d *viewer, QKeyEvent *event)
  {
  Q_UNUSED(viewer)
  Q_UNUSED(event)
  }




