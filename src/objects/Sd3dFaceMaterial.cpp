#include "Sd3dFaceMaterial.h"
#include "Sd3dPoint.h"

#include <QColor>


Sd3dFaceMaterial::Sd3dFaceMaterial() :
  mAmbientIntensity(0.2),
  mShininnes(0.2)
  {
  setDiffuseColor( 0.8, 0.8, 0.8 );
  setEmissiveColor( 0.0, 0.0, 0.0 );
  setSpecularColor( 0.0, 0.0, 0.0 );
  setTransparency(0.0);
  }



void Sd3dFaceMaterial::setColor(QColor color)
  {
  setDiffuseColor( color.redF(), color.greenF(), color.blueF() );
  //setTransparency( color.alphaF() );
  }



void Sd3dFaceMaterial::paint(QOpenGLFunctions_2_0 *f) const
  {
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mAmbientColor );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mDiffuseColor );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, mEmissiveColor );
  f->glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mShininnes );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mSpecularColor );
  }


static void jsonFloatMcm( SvJsonWriter &js, const QString &key, float v )
  {
  js.jsonInt( key, floatMmToIntMcm(v) );
  }

static void jsonFloatMcm( const SvJsonReader &js, const QString &key, float &v )
  {
  int i;
  js.jsonInt( key, i );
  v = intMcmToFloatMm(i);
  }


static void jsonColor3d( SvJsonWriter &js, const QString &prefix, const float *color )
  {
  //Convert color to rgba
  js.jsonString( prefix, QString::number( qRgba(color[0]*255.0, color[1]*255.0, color[2]*255.0, color[3]*255.0), 16 ) );
  }


static void jsonColor3d( const SvJsonReader &js, const QString &prefix, float *color )
  {
  QRgb rgba = js.object().value( prefix ).toString().toUInt( nullptr, 16 );
  color[0] = qRed( rgba );
  color[0] /= 255.0;
  color[1] = qGreen( rgba );
  color[1] /= 255.0;
  color[2] = qBlue( rgba );
  color[2] /= 255.0;
  color[3] = qAlpha( rgba );
  color[3] /= 255.0;
  }





//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void Sd3dFaceMaterial::json(SvJsonWriter &js) const
  {
  jsonFloatMcm( js, QString("mAmInt"), mAmbientIntensity );
  jsonColor3d( js, QStringLiteral("mDf"), mDiffuseColor );
  jsonColor3d( js, QStringLiteral("mEm"), mEmissiveColor );
  jsonFloatMcm( js, QString("mShin"), mShininnes );
  jsonColor3d( js, QStringLiteral("mSp"), mSpecularColor );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void Sd3dFaceMaterial::json(const SvJsonReader &js)
  {
  jsonFloatMcm( js, QString("mAmInt"), mAmbientIntensity );
  jsonColor3d( js, QStringLiteral("mDf"), mDiffuseColor );
  jsonColor3d( js, QStringLiteral("mEm"), mEmissiveColor );
  jsonFloatMcm( js, QString("mShin"), mShininnes );
  jsonColor3d( js, QStringLiteral("mSp"), mSpecularColor );
  buildAmbientColor();
  }







void Sd3dFaceMaterial::buildAmbientColor()
  {
  for( int i = 0; i < 3; i++ )
    mAmbientColor[i] = mDiffuseColor[i] * mAmbientIntensity;
  }


