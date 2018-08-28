/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Single pad description. On this description pads created on demand.
*/
#ifndef SDPAD_H
#define SDPAD_H

#include "SdStratum.h"
#include "SdPoint.h"
#include "SdPolyWindowList.h"

#include <QJsonObject>
#include <QPolygonF>
#include <QMap>

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

    //Return polygon of pad
    QPolygonF   polygon( SdPoint p, int addon ) const;

    //Append pad as window
    void        appendWindow( SdPolyWindowList &dest, SdPoint p, int gap, const QTransform &t) const;

    //Create textual pad description
    QString     description() const;

    //Clear to create default pad
    void        clear();

    bool        isEmpty() const { return mHeight < 0; }

    friend class SdDPadMaster;
  };


typedef QMap<QString,SdPad> SdPadMap;

#endif // SDPAD_H
