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


static void writeColor( QJsonObject &obj, const QString &prefix, const float *color )
  {
  //Convert color to rgba
  obj.insert( prefix, QString::number( qRgba(color[0]*255.0, color[1]*255.0, color[2]*255.0, color[3]*255.0), 16 ) );
  }


static void readColor( const QJsonObject &obj, const QString &prefix, float *color )
  {
  QRgb rgba = obj.value( prefix ).toString().toUInt( nullptr, 16 );
  color[0] = qRed( rgba );
  color[0] /= 255.0;
  color[1] = qGreen( rgba );
  color[1] /= 255.0;
  color[2] = qBlue( rgba );
  color[2] /= 255.0;
  color[3] = qAlpha( rgba );
  color[3] /= 255.0;
  }


void Sd3dFaceMaterial::write(QJsonObject &obj) const
  {
  obj.insert( QString("mAmInt"), floatMmToIntMcm( mAmbientIntensity )  );
  writeColor( obj, QStringLiteral("mDf"), mDiffuseColor );
  writeColor( obj, QStringLiteral("mEm"), mEmissiveColor );
  obj.insert( QString("mShin"), floatMmToIntMcm( mShininnes )  );
  writeColor( obj, QStringLiteral("mSp"), mSpecularColor );
  }



void Sd3dFaceMaterial::read(const QJsonObject &obj)
  {
  mAmbientIntensity = intMcmToFloatMm( obj.value( QString("mAmInt") ).toInt() );
  readColor( obj, QStringLiteral("mDf"), mDiffuseColor );
  readColor( obj, QStringLiteral("mEm"), mEmissiveColor );
  mShininnes = intMcmToFloatMm( obj.value( QString("mShin") ).toInt() );
  readColor( obj, QStringLiteral("mSp"), mSpecularColor );
  buildAmbientColor();
  }



void Sd3dFaceMaterial::buildAmbientColor()
  {
  for( int i = 0; i < 3; i++ )
    mAmbientColor[i] = mDiffuseColor[i] * mAmbientIntensity;
  }


