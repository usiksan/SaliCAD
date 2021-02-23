/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function build circle region
*/
#ifndef SDM3DFUNREGIONCIRCLE_H
#define SDM3DFUNREGIONCIRCLE_H

#include "SdM3dFunction.h"

class SdM3dFunRegionCircle : public SdM3dFunction
  {
  public:
    SdM3dFunRegionCircle();

    // SdM3dValue interface
  public:
    virtual SdM3dRegion toRegion() const override;

    //!
    //! \brief regionCircle This function builds circle region on base radius with center at 0
    //! \param radius       Radius of builded circle
    //! \return             Circle region on XY plane
    //!
    static  SdM3dRegion regionCircle( float radius );
  };

#endif // SDM3DFUNREGIONCIRCLE_H
