/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdQuadrangle.h"

SdQuadrangle::SdQuadrangle() : p1(), p2(), p3(), p4()
  {

  }

SdQuadrangle::SdQuadrangle(SdPoint s1, SdPoint s2, SdPoint s3, SdPoint s4) :
  p1(s1), p2(s2), p3(s3), p4(s4)
  {

  }

SdQuadrangle::SdQuadrangle(int x, int y, int w, int h)
  {
  set( SdRect(x,y,w,h) );
  }


void SdQuadrangle::set(SdRect r)
  {
  p1 = r.getTopLeft();
  p2 = r.getTopRight();
  p3 = r.getBottomRight();
  p4 = r.getBottomLeft();
  }

//QJsonObject SdQuadrangle::write() const
//  {
//  QJsonObject obj;
//  p1.write( QStringLiteral("p1"), obj );
//  p2.write( QStringLiteral("p2"), obj );
//  p3.write( QStringLiteral("p3"), obj );
//  p4.write( QStringLiteral("p4"), obj );
//  return obj;
//  }

//void SdQuadrangle::write(const QString name, QJsonObject &dest) const
//  {
//  dest.insert( name, write() );
//  }

//void SdQuadrangle::read(const QJsonObject obj)
//  {
//  p1.read( QStringLiteral("p1"), obj );
//  p2.read( QStringLiteral("p2"), obj );
//  p3.read( QStringLiteral("p3"), obj );
//  p4.read( QStringLiteral("p4"), obj );
//  }

//void SdQuadrangle::read(const QString name, const QJsonObject src)
//  {
//  read( src.value(name).toObject() );
//  }

