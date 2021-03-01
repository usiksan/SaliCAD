/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds rectangle region with center in 0
*/
#ifndef SDM3DFUNREGIONRECT_H
#define SDM3DFUNREGIONRECT_H

#include "SdM3dFunction.h"

class SdM3dFunRegionRect : public SdM3dFunction
  {
  public:
    SdM3dFunRegionRect();

    // SdM3dValue interface
  public:
    virtual SdM3dRegion toRegion() const override;

    //!
    //! \brief regionRect Builds rectangle region with center in 0
    //! \param width      Width (X) of rectangle (in flat view)
    //! \param height     Height (Y) of rectangle (in flat view)
    //! \return           Rectangle region with center in 0
    //!
    static  SdM3dRegion regionRect( float width, float height, QVector3D offset = QVector3D(0.0,0.0,0.0) );
  };

#endif // SDM3DFUNREGIONRECT_H
