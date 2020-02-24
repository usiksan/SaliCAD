#ifndef SD3DPARALLELEPIPED_H
#define SD3DPARALLELEPIPED_H

#include "Sd3dObject.h"
#include "SdPoint3d.h"

class Sd3dParallelepiped : public Sd3dObject
  {
    SdPoint3d mTopPlane[4];
    SdPoint3d mBotPlane[4];
  public:
    Sd3dParallelepiped();

    void setAsFlatXY( SdRect &projection, int z );
  };

#endif // SD3DPARALLELEPIPED_H
