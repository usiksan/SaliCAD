#include "Sd3dFaceMaterial.h"

//mAmbientIntensity(0.2),
//mDiffuseColor( 0.8, 0.8, 0.8 ),
//mEmissiveColor(),
//mShininnes(0.2),
//mSpecularColor(),
//mTransparency(0.0)

Sd3dFaceMaterial::Sd3dFaceMaterial() :
  mAmbientIntensity(0.2),
  mShininnes(0.2)
  {
  setDiffuseColor( 0.8, 0.8, 0.8 );
  setEmissiveColor( 0.0, 0.0, 0.0 );
  setSpecularColor( 0.0, 0.0, 0.0 );
  setTransparency(0.0);
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
  obj.insert( prefix + QStringLiteral("R"), color[0] );
  obj.insert( prefix + QStringLiteral("G"), color[1] );
  obj.insert( prefix + QStringLiteral("B"), color[2] );
  obj.insert( prefix + QStringLiteral("A"), color[3] );
  }


static void readColor( const QJsonObject &obj, const QString &prefix, float *color )
  {
  color[0] = obj.value( prefix + QStringLiteral("R") ).toDouble();
  color[1] = obj.value( prefix + QStringLiteral("G") ).toDouble();
  color[2] = obj.value( prefix + QStringLiteral("B") ).toDouble();
  color[3] = obj.value( prefix + QStringLiteral("A") ).toDouble();
  }


void Sd3dFaceMaterial::write(QJsonObject &obj) const
  {
  obj.insert( QString("mAmInt"), mAmbientIntensity );
  writeColor( obj, QStringLiteral("mAm"), mAmbientColor );
  writeColor( obj, QStringLiteral("mDf"), mDiffuseColor );
  writeColor( obj, QStringLiteral("mEm"), mEmissiveColor );
  obj.insert( QString("mShin"), mShininnes );
  writeColor( obj, QStringLiteral("mSp"), mSpecularColor );
  }



void Sd3dFaceMaterial::read(const QJsonObject &obj)
  {
  mAmbientIntensity = obj.value( QString("mAmInt") ).toDouble();
  readColor( obj, QStringLiteral("mAm"), mAmbientColor );
  readColor( obj, QStringLiteral("mDf"), mDiffuseColor );
  readColor( obj, QStringLiteral("mEm"), mEmissiveColor );
  mShininnes = obj.value( QString("mShin") ).toDouble();
  readColor( obj, QStringLiteral("mSp"), mSpecularColor );
  }


