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
#include "Sd3drMaterial.h"






void Sd3drMaterial::setAmbientIntensity(float am)
  {
  float fcolor[4];
  colorToFloat( mDiffuse, fcolor );

  for( int i = 0; i < 3; i++ )
    fcolor[i] *= am;

  mAmbient = QColor( fcolor[0] * 255.0, fcolor[1] * 255.0, fcolor[2] * 255.0, fcolor[3] * 255.0 );
  }




void Sd3drMaterial::draw(QOpenGLFunctions_2_0 *f) const
  {
  float fcolor[4];
  colorToFloat( mAmbient, fcolor );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, fcolor );

  colorToFloat( mDiffuse, fcolor );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, fcolor );

//  Sd3dDraw::colorToFloat( mEmissive, fcolor );
//  f->glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, fcolor );

//  f->glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mShininnes );

//  Sd3dDraw::colorToFloat( mSpecular, fcolor );
//  f->glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, fcolor );
  }



void Sd3drMaterial::setColor(QColor color)
  {
  mDiffuse = color;
  }



//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void Sd3drMaterial::json(SvJsonWriter &js) const
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
void Sd3drMaterial::json(const SvJsonReader &js)
  {
  js.jsonColor( QStringLiteral("Ambient"), mAmbient );
  js.jsonColor( QStringLiteral("Diffuse"), mDiffuse );
  js.jsonColor( QStringLiteral("Emissive"), mEmissive );
  js.jsonColor( QStringLiteral("Specular"), mSpecular );
  js.jsonDouble( QStringLiteral("Shininnes"), mShininnes );
  }



void Sd3drMaterial::colorToFloat(QColor col, float *fcolor)
  {
  fcolor[0] = col.redF();
  fcolor[1] = col.greenF();
  fcolor[2] = col.blueF();
  fcolor[3] = col.alphaF();
  }



