#ifndef SDM3DFUNMODELWALL_H
#define SDM3DFUNMODELWALL_H

#include "SdM3dFunction.h"

class SdM3dFunModelWall : public SdM3dFunction
  {
  public:
    SdM3dFunModelWall();

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override;

    static  SdM3dModel modelWall(SdM3dRegion region, QVector3D grow, QColor color , bool close);

    //!
    //! \brief modelWalls Builds walls on base bottom and top regions. Walls builded with color
    //! \param bottom     Bottom region of walls
    //! \param top        Top region of walls
    //! \param color      Color of faces for the walls
    //! \param close      If true then append wall with n-1 and 0 index vertex
    //! \return           Model of walls
    //!
    static  SdM3dModel modelWalls( SdM3dRegion bottom, SdM3dRegion top, QColor color, bool close );
  };

#endif // SDM3DFUNMODELWALL_H
