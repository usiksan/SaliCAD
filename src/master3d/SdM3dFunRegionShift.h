/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function moves the region in the direction perpendicular to the plane of the region by the specified amount
*/
#ifndef SDM3DFUNREGIONSHIFT_H
#define SDM3DFUNREGIONSHIFT_H

#include "SdM3dFunction.h"

class SdM3dFunRegionShift : public SdM3dFunction
  {
  public:
    SdM3dFunRegionShift();

    // SdM3dValue interface
  public:
    virtual SdM3dRegion toRegion() const override;

    //!
    //! \brief regionShift The function moves the region in the direction perpendicular
    //!                    to the plane of the region by the specified shift amount
    //! \param region      Source region to move
    //! \param shift       Shift amount
    //! \return            Moved region
    //!
    static  SdM3dRegion regionShift( SdM3dRegion region, float shift );
  };

#endif // SDM3DFUNREGIONSHIFT_H
