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
#include "SvJsonIO.h"

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
    //! \brief Sd3dFace Builds triangle face with three vertex's and face color
    //! \param p[0,1,2] Vertexies
    //! \param color    Face color
    //!
    Sd3dFace( QVector3D p0, QVector3D p1, QVector3D p2, QColor color );


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

    //!
    //! \brief translate Translate this face region with offset
    //! \param offset    Offset vector of translation
    //! \return          Translated face
    //!
    Sd3dFace    translate( QVector3D offset ) const { return Sd3dFace( sd3dRegionTranslate( mContour, offset ), mColor ); }

    //!
    //! \brief translateColor Translate this face region with offset and change face color
    //! \param offset         Offset vector of translation
    //! \param color          New color of face
    //! \return               Translated and colored face
    //!
    Sd3dFace    translateColor( QVector3D offset, QColor color ) const { return Sd3dFace( sd3dRegionTranslate( mContour, offset ), color ); }

    //!
    //! \brief shift  Shifts face in perpendecular direction with offset amount
    //! \param offset Shift offset amount
    //! \return       Shifted face
    //!
    Sd3dFace    shift( float offset ) const { return Sd3dFace( sd3dRegionShift( mContour, offset ), mColor ); }

    //!
    //! \brief map    Build face from this converted with matrix
    //! \param matrix Matrix of conversion
    //! \return       Converted face
    //!
    Sd3dFace    map( const QMatrix4x4 &matrix ) const { return Sd3dFace( sd3dRegionMap( mContour, matrix ), mColor ); }

    //!
    //! \brief mapInPlace Convert this face with matrix
    //! \param matrix     Matrix of conversion
    //!
    void        mapInPlace( const QMatrix4x4 &matrix ) { sd3dRegionMapInPlace( mContour, matrix ); }

    //!
    //! \brief volume Append face to volume
    //! \param vol    Volume source and result
    //!
    void        volume(QMatrix2x3 &vol ) const;
  };

#endif // SD3DFACE_H
