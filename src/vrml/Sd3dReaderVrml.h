#ifndef SD3DREADERVRML_H
#define SD3DREADERVRML_H

#include "SdScanerMultyline.h"

#include <QList>

struct Sd3dVrmlColor {
    double mRed;
    double mGreen;
    double mBlue;
  };

using Sd3dVrmlColorList = QList<Sd3dVrmlColor>;


struct Sd3dVrmlPoint {
    double mX;
    double mY;
    double mZ;
  };

using Sd3dVrmlPointList = QList<Sd3dVrmlPoint>;

using Sd3dVrmlIntList = QList<int>;

using Sd3dVrmlDoubleList = QList<double>;

class Sd3dReaderVrml : public SdScanerMultyline
  {
    Sd3dVrmlColorList mDiffuseColor;
    Sd3dVrmlColorList mEmissiveColor;
    Sd3dVrmlColorList mSpecularColor;
    Sd3dVrmlDoubleList mAmbientIntensity;
    Sd3dVrmlDoubleList mTransparency;
    Sd3dVrmlDoubleList mShininess;
    Sd3dVrmlPointList mPoint;
    Sd3dVrmlIntList   mCoordIndex;
    Sd3dVrmlPointList mNormal;
    Sd3dVrmlIntList   mNormalIndex;
    Sd3dVrmlColorList mColor;

  public:
    Sd3dReaderVrml();
  };

#endif // SD3DREADERVRML_H
