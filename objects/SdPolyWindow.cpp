/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPolyWindow.h"
#include "SdRect.h"
#include "SdQuadrangle.h"

SdPolyWindow::SdPolyWindow() :
  radius(-2), //Empty window
  p1(),p2(),p3(),p4()
  {

  }




SdPolyWindow::SdPolyWindow(SdCircle circle, int gap) :
  radius( circle.mRadius + gap ),
  p1(circle.mCenter),p2(),p3(),p4()
  {

  }




SdPolyWindow::SdPolyWindow(SdRect r, int gap) :
  radius(-1),
  p1( r.getTopLeft() ),
  p2( r.getTopRight() ),
  p3( r.getBottomRight() ),
  p4( r.getBottomLeft() )
  {
  p1.move( SdPoint(-gap,-gap) );
  p2.move( SdPoint(gap,-gap) );
  p3.move( SdPoint(gap,gap) );
  p4.move( SdPoint(-gap,gap) );
  }

SdPolyWindow::SdPolyWindow(SdPoint a, SdPoint b, int gap) :
  radius(-1),p1(),p2(),p3(),p4()
  {
  SdQuadrangle q( a.x(), a.y()-gap, a.getDistance(b), gap * 2 );
  q.rotate( a, b.getAngle(a) );
  p1 = q.p1;
  p2 = q.p2;
  p3 = q.p3;
  p4 = q.p4;
  }





QJsonObject SdPolyWindow::write() const
  {
  QJsonObject obj;
  obj.insert( QStringLiteral("r"), radius );
  p1.write( QStringLiteral("p1"), obj );
  p2.write( QStringLiteral("p2"), obj );
  p3.write( QStringLiteral("p3"), obj );
  p4.write( QStringLiteral("p4"), obj );
  return obj;
  }

void SdPolyWindow::read(const QJsonObject obj)
  {
  radius = obj.value( QStringLiteral("r") ).toInt();
  p1.read( QStringLiteral("p1"), obj );
  p2.read( QStringLiteral("p2"), obj );
  p3.read( QStringLiteral("p3"), obj );
  p4.read( QStringLiteral("p4"), obj );
  }



