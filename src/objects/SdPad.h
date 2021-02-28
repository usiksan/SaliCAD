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
#include <QOpenGLFunctions_2_0>

class SdPItemPlate;
class SdContext;

class SdPad
  {
    qint32 mCenterX;          //Center of circle or rectangle
    qint32 mCenterY;
    qint32 mDiametrWidth;     //Diametr of circle or rectangle dimensions
    qint32 mHeight;
    qint32 mMaskThreshold;    //Mask threshold. Calculation: maskSize = size + mMaskThreshold
    qint32 mStencilThreshold; //Stensil threshold. Calculation: stensilSize = size + mMaskThreshold
    qint32 mStencilRows;      //If rows or colons > 1 then stensil area divided on array of aperture with
    qint32 mStencilCols;      // width and height
    qint32 mStencilWidth;
    qint32 mStencilHeight;
    qint32 mHoleDiametr;
    qint32 mHoleLength;       //Lenght for slot holes. If zero then holes is circle
    qint32 mSlotAngle;        //Angle for slot. 0 degree is horizontal hole
    //Not saved. Circle determine by mHeight = 0.
    bool   mIsCircle;
  public:
    SdPad();

    QJsonObject write() const;
    void        read( const QJsonObject obj);

    void        draw(SdContext *dcx, SdPoint p, int stratum ) const;

    //!
    //! \brief draw3d Draws object in 3d space
    //! \param f      3d draw functions with predefined 3d context
    //! \param p      Point of center of pad
    //!
    void        draw3d( QOpenGLFunctions_2_0 *f, SdPoint p ) const;

    //Return polygon of pad
    QPolygonF   polygon( SdPoint p, int addon ) const;

    //Append pad as window
    void        appendWindow( SdPolyWindowList &dest, SdPoint p, int gap, const QTransform &t) const;

    //Return over pad circle radius
    int         overCircleRadius() const;

    //Create textual pad description
    QString     description() const;

    //Create textual pad name
    QString     name() const;

    //Create pad on name base
    void        parse( const QString nm );

    //Clear to create default pad
    void        clear();

    //Calculate points of slot hole
    void        slotPoints( SdPoint &a, SdPoint &b ) const;

    SdPoint     center() const { return SdPoint(mCenterX,mCenterY); }

    bool        isEmpty() const { return mHeight < 0; }

    bool        isThrough() const { return mHoleDiametr > 0; }

    friend class SdDPadMaster;
  };


typedef QMap<QString,SdPad> SdPadMap;

#endif // SDPAD_H
