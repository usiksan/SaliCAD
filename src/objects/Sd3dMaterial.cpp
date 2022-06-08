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
#include "Sd3dMaterial.h"
#include "Sd3dDraw.h"






void Sd3dMaterial::setAmbientIntensity(float am)
  {
  float fcolor[4];
  Sd3dDraw::colorToFloat( mDiffuse, fcolor );

  for( int i = 0; i < 3; i++ )
    fcolor[i] *= am;

  mAmbient = QColor( fcolor[0] * 255.0, fcolor[1] * 255.0, fcolor[2] * 255.0, fcolor[3] * 255.0 );
  }




void Sd3dMaterial::draw(QOpenGLFunctions_2_0 *f) const
  {
  float fcolor[4];
  Sd3dDraw::colorToFloat( mAmbient, fcolor );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, fcolor );

  Sd3dDraw::colorToFloat( mDiffuse, fcolor );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, fcolor );

//  Sd3dDraw::colorToFloat( mEmissive, fcolor );
//  f->glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, fcolor );

//  f->glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mShininnes );

//  Sd3dDraw::colorToFloat( mSpecular, fcolor );
//  f->glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, fcolor );
  }



void Sd3dMaterial::setColor(QColor color)
  {
  mDiffuse = color;
  }



//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void Sd3dMaterial::json(SvJsonWriter &js) const
  {
  js.jsonColor( QStringLiteral("Ambient"), mAmbient );
  js.jsonColor( QStringLiteral("Diffuse"), mDiffuse );
  js.jsonColor( QStringLiteral("Emissive"), mEmissive );
  js.jsonColor( QStringLiteral("Specular"), mSpecular );
  js.jsonDouble( QStringLiteral("Shininnes"), mShininnes );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void Sd3dMaterial::json(const SvJsonReader &js)
  {
  js.jsonColor( QStringLiteral("Ambient"), mAmbient );
  js.jsonColor( QStringLiteral("Diffuse"), mDiffuse );
  js.jsonColor( QStringLiteral("Emissive"), mEmissive );
  js.jsonColor( QStringLiteral("Specular"), mSpecular );
  js.jsonDouble( QStringLiteral("Shininnes"), mShininnes );
  }



