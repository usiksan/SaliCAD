/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D face material. Material of which face is made
*/
#ifndef SD3DMATERIAL_H
#define SD3DMATERIAL_H


#include "SvLib/SvJsonIO.h"

#include <QJsonObject>
#include <QOpenGLFunctions_2_0>
#include <QList>


struct Sd3dMaterial
  {
    QColor        mAmbient;
    QColor        mDiffuse;
    QColor        mSpecular;
    QColor        mEmissive;
    double        mShininnes;

    Sd3dMaterial() : mShininnes(0) {}

    Sd3dMaterial( QColor amb, QColor dif, QColor spec ) : mAmbient(amb), mDiffuse(dif), mSpecular(spec), mEmissive(0,0,0), mShininnes(0.2) {}

    void setAmbientIntensity( float am );

    void draw( QOpenGLFunctions_2_0 *f ) const;


    void setColor( QColor color );


    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    void json( SvJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    void json( const SvJsonReader &js);

  };

using Sd3ColorList = QList<Sd3dMaterial>;

#endif // SD3DMATERIAL_H
