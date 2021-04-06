/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Move for simple view 3d scene. Does nothing more.
*/
#include "Sd3dModeView.h"
#include "SdModeIdents.h"

Sd3dModeView::Sd3dModeView()
  {

  }



bool Sd3dModeView::draw3d(QOpenGLFunctions_2_0 *f)
  {
  Q_UNUSED(f)
  return false;
  }




int Sd3dModeView::modeId() const
  {
  return MD_3D_VIEW;
  }
