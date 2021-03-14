/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D face. Face is flat surface bounded by region
*/
#ifndef SD3DFACE_H
#define SD3DFACE_H

#include "Sd3dRegion.h"

#include <QColor>
#include <QJsonObject>

struct Sd3dFace
  {
    Sd3dRegion mContour; //!< Face contour
    QColor     mColor;   //!< Face color

    Sd3dFace() : mContour(), mColor() {}
    Sd3dFace( Sd3dRegion &&region, QColor color ) : mContour(region), mColor(color) {}
    Sd3dFace( const Sd3dRegion &region, QColor color ) : mContour(region), mColor(color) {}

    //!
    //! \brief write Writes face to JSON object
    //! \return      JSON object with face
    //!
    QJsonObject write() const;

    //!
    //! \brief read Read face from JSON object
    //! \param obj  JSON object with face
    //!
    void        read( const QJsonObject &obj );

    //!
    //! \brief translate Translate this face region with offset
    //! \param offset    Offset vector of translation
    //! \return          Translated face
    //!
    Sd3dFace    translate( QVector3D offset );
  };

#endif // SD3DFACE_H
