#include "Sd3dFlat.h"
#include <QColor>

Sd3dFlat::Sd3dFlat()
  {

  }

void Sd3dFlat::buildParallelepiped(int x, int y, int z, unsigned color)
  {

  }


QString Sd3dFlat::getType() const
  {
  return QStringLiteral( SD_TYPE_3D_FLAT );
  }




void Sd3dFlat::cloneFrom(const SdObject *src)
  {
  Sd3dGraph::cloneFrom( src );
  SdPtrConst<Sd3dFlat> flat(src);
  if( flat.isValid() ) {
    mRegion      = flat->mRegion;
    mWidthVector = flat->mWidthVector;
    mColor       = flat->mColor;
    }
  }




void Sd3dFlat::writeObject(QJsonObject &obj) const
  {
  Sd3dGraph::writeObject( obj );
  obj.insert( QStringLiteral("color"), static_cast<int>(mColor) );
  mWidthVector.write( QStringLiteral("widthVector"), obj );
  QJsonArray ar;
  for( auto p : mRegion )
    ar.append( p.write() );
  obj.insert( QStringLiteral("region"), ar );
  }




void Sd3dFlat::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  Sd3dGraph::readObject( map, obj );
  mColor = static_cast<quint32>(obj.value(QStringLiteral("color")).toInt() );
  mWidthVector.read( QStringLiteral("widthVector"), obj );
  QJsonArray ar = obj.value( QStringLiteral("region") ).toArray();
  mRegion.clear();
  for( auto val : ar ) {
    Sd3dPoint p;
    p.read( val.toObject() );
    mRegion.append( p );
    }
  }





void Sd3dFlat::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  //1. We draw base surfase with region
  f->glColor3b( qRed(mColor) >> 1, qGreen(mColor) >> 1, qBlue(mColor) >> 1 );
  f->glBegin(GL_POLYGON);
  for( const Sd3dPoint &p : mRegion )
    p.vertex( f );
  f->glEnd();

  //2. We draw second surfase with region shifted on widthVector
  f->glBegin(GL_POLYGON);
  for( const Sd3dPoint &p : mRegion )
    Sd3dPoint(p + mWidthVector).vertex( f );
  f->glEnd();

  //3. We draw side surfases
  for( int i = 0; i < mRegion.count(); i++ ) {
    Sd3dPoint p1 = mRegion.at(i);
    Sd3dPoint p2;
    if( i == 0 )
      //Wrapped point
      p2 = mRegion.last();
    else
      //Previous point
      p2 = mRegion.at(i - 1);
    Sd3dPoint p3 = p2 + mWidthVector;
    Sd3dPoint p4 = p1 + mWidthVector;
    //So, p1-p2 base edge, p3-p4 second edge
    f->glBegin(GL_POLYGON);
    p1.vertex(f);
    p2.vertex(f);
    p3.vertex(f);
    p4.vertex(f);
    f->glEnd();
    }
  }

