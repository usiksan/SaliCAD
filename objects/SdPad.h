#ifndef SDPAD_H
#define SDPAD_H

#include "SdStratum.h"
#include "SdPoint.h"

#include <QJsonObject>
#include <QPolygon>

class SdPItemPlate;
class SdContext;

class SdPad
  {
    qint32 mCenterX;          //Center of circle or rectangle
    qint32 mCenterY;
    qint32 mDiametrWidth;     //Diametr of circle or rectangle dimensions
    qint32 mHeight;
    qint32 mMaskThreshold;    //Mask threshold. Calculation: maskSize = size + mMaskThreshold
    qint32 mStensilThreshold; //Stensil threshold. Calculation: stensilSize = size + mMaskThreshold
    qint32 mStensilRows;      //If rows or colons > 1 then stensil area divided on array of aperture with
    qint32 mStensilCols;      // width and height
    qint32 mStensilWidth;
    qint32 mStensilHeight;
    qint32 mHoleDiametr;
    //Not saved. Circle determine by mHeight = 0.
    bool   mIsCircle;
  public:
    SdPad();

    QJsonObject write() const;
    void        read( const QJsonObject obj);

    void        draw(SdContext *dcx, SdPoint p, int stratum ) const;

    QPolygon    polygon( SdPoint p );
  };

#endif // SDPAD_H
