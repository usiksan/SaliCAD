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

#include "SdPropStratum.h"
#include "SdPoint.h"
#include "SdPolyWindowList.h"
#include "3dr/Sd3drModel.h"

#include <QJsonObject>
#include <QPolygonF>
#include <QMap>
#include <QOpenGLFunctions_2_0>

class SdPItemPlate;
class SdContext;
class SdPad
  {
    qint32    mCenterX;          //!< Center of circle or rectangle X coordinate
    qint32    mCenterY;          //!< Center of circle or rectangle Y coordinate
    qint32    mDiametrWidth;     //!< Diameter of circle or rectangle width
    qint32    mHeight;           //!< Rectangle height (if -1 then circle)
    qint32    mMaskThreshold;    //!< Mask threshold. Calculation: maskSize = size + mMaskThreshold
    qint32    mStencilThreshold; //!< Stencil threshold. Calculation: stencilSize = size - mStencilThreshold
    qint32    mStencilRows;      //!< Number of rows for stencil array
    qint32    mStencilCols;      //!< Number of columns for stencil array
    qint32    mStencilWidth;     //!< Width of individual stencil aperture
    qint32    mStencilHeight;    //!< Height of individual stencil aperture
    qint32    mHoleDiametr;      //!< Hole diameter (if zero then no hole)
    qint32    mHoleLength;       //!< Length for slot holes (if zero then circular hole)
    qint32    mAngle;            //!< Pad and hole rotation angle in 0.001 degrees.
                                 //!< 0 is horizontal slot orientation. Positive values clockwise
    qint32    mRoundRectRatio;   //!< Rounded rectangle ratio in hundredths (0 if not rounded)
    bool      mEllipse;          //!< True for elliptical pad shape
    //Not saved. Circle determine by mHeight = 0.
    bool      mIsCircle;         //!< True for circular pad (legacy)
    QPolygonF mCachePad;         //!< Cached polygon for pad
    QPolygonF mCacheMask;        //!< Cached polygon for mask
    QPolygonF mCacheStencil;     //!< Cached polygon for stencil
    Sd3drModel mModel;           //!< 3D model representation
  public:
    SdPad();

    //!
    //! \brief draw Draws pad in 2D context
    //! \param dcx   Drawing context
    //! \param p     Center position of pad
    //! \param stratum Stratum for layer selection
    //!
    void        draw(SdContext *dcx, SdPoint p, int stratum ) const;

    //!
    //! \brief draw3d Draws object in 3d space
    //! \param f      3d draw functions with predefined 3d context
    //! \param p      Point of center of pad
    //!
    void        draw3d( QOpenGLFunctions_2_0 *f, SdPoint p ) const;

    //!
    //! \brief polygon Returns polygon for pad with optional size adjustment
    //! \param p       Center position of pad
    //! \param addon   Additional size to add to pad (positive for enlargement, negative for reduction)
    //! \return        Polygon representing pad shape (may be transformed by rotation later)
    //!
    QPolygonF   polygon( SdPoint p, int addon ) const;

    //!
    //! \brief appendWindow Appends pad as window to list
    //! \param dest         Destination window list
    //! \param p            Center position of pad
    //! \param gap          Gap around pad
    //! \param t            Transform to apply
    //!
    void        appendWindow( SdPolyWindowList &dest, SdPoint p, int gap, const QTransform &t) const;

    //!
    //! \brief appendPadHoles Accumulates holes description into faceList
    //! \param p              Position of pad
    //! \param model          Model which accumulates coordinate vertexes
    //! \param faceList       Face list for holding holes (single face for single hole)
    //! \param stratum        Stratum for layers (unused)
    //! \param map            Base map for holes conversion (applied after local transforms)
    //!
    void        appendPadHoles( SdPoint p, Sd3drModel &model, Sd3drFaceList &faceList, SdPropStratum stratum, const QMatrix4x4 &map ) const;

    //!
    //! \brief overCircleRadius Returns over pad circle radius
    //! \return                 Radius of circle that encloses the pad
    //!
    int         overCircleRadius() const;

    //!
    //! \brief description Creates textual pad description
    //! \return            Human-readable pad description
    //!
    QString     description() const;

    //!
    //! \brief name Creates textual pad name
    //! \return     Pad name for serialization
    //!
    QString     name() const;

    //!
    //! \brief parse Creates pad from name string
    //! \param nm   Name string to parse
    //!
    void        parse( const QString nm );

    //!
    //! \brief clear Resets pad to default state
    //!
    void        clear();

    //!
    //! \brief slotPoints Calculates points of slot hole
    //! \param a          Starting point of slot
    //! \param b          Ending point of slot
    //!
    void        slotPoints( SdPoint &a, SdPoint &b ) const;

    //!
    //! \brief center Returns pad center point
    //! \return       Center point
    //!
    SdPoint     center() const { return SdPoint(mCenterX,mCenterY); }

    //!
    //! \brief rebuildCache Rebuilds cached polygons for pad, mask and stencil
    //!                     Cache is built only for new shapes (ellipse, rounded rect)
    //!                     For legacy shapes or when mask/stencil absent, caches remain empty
    //!
    void        rebuildCache();

    //!
    //! \brief isEmpty Checks if pad is empty
    //! \return        True if pad has no size
    //!
    bool        isEmpty() const { return mHeight < 0; }

    //!
    //! \brief isThrough Checks if pad has through hole
    //! \return          True if pad has hole
    //!
    bool        isThrough() const { return mHoleDiametr > 0; }

    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    void        json( SvJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    void        json( const SvJsonReader &js);

    friend class SdDPadMaster;
  };

typedef QMap<QString,SdPad> SdPadMap;

#endif // SDPAD_H
