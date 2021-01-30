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
#ifndef SD3DFACEMATERIAL_H
#define SD3DFACEMATERIAL_H

#include <QJsonObject>
#include <QOpenGLFunctions_2_0>

class Sd3dFaceMaterial
  {
    float mAmbientIntensity;
    float mAmbientColor[4];
    float mDiffuseColor[4];
    float mEmissiveColor[4];
    float mShininnes;
    float mSpecularColor[4];
  public:
    Sd3dFaceMaterial();

    void setAmbientIntensity( float am ) { mAmbientIntensity = am; buildAmbientColor(); }

    void setDiffuseColor( float r, float g, float b ) { mDiffuseColor[0] = r; mDiffuseColor[1] = g; mDiffuseColor[2] = b; buildAmbientColor(); }

    void setEmissiveColor( float r, float g, float b ) { mEmissiveColor[0] = r; mEmissiveColor[1] = g; mEmissiveColor[2] = b; }

    void setShininnes( float s ) { mShininnes = s; }

    void setSpecularColor( float r, float g, float b ) { mSpecularColor[0] = r; mSpecularColor[1] = g; mSpecularColor[2] = b; }

    void setTransparency( float t ) { mAmbientColor[3] = mDiffuseColor[3] = mEmissiveColor[3] = mSpecularColor[3] = 1.0 - t; }

    void paint( QOpenGLFunctions_2_0 *f ) const;

    void write( QJsonObject &obj ) const;

    void read( const QJsonObject &obj );

  private:
    void buildAmbientColor();

  };

#endif // SD3DFACEMATERIAL_H
