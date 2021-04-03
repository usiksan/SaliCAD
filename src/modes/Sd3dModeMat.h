/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base class for all 3d view modes used matrix mapping
*/
#ifndef SD3DMODEMAT_H
#define SD3DMODEMAT_H

#include "objects/SdPItemPart.h"
#include "Sd3dMode.h"

#include <QMatrix4x4>

class Sd3dModeMat : public Sd3dMode
  {
  protected:
    SdPItemPart *mPart;   //!< Part on which applied this mode
  public:
    Sd3dModeMat( SdPItemPart *part );

  };

#endif // SD3DMODEMAT_H
