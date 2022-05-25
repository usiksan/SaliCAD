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



void SdPolyWindow::buildPolygon()
  {
  mPolygon.clear();
  mPolygon.append( p1.toPointF() );
  mPolygon.append( p2.toPointF() );
  mPolygon.append( p3.toPointF() );
  mPolygon.append( p4.toPointF() );
  }



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

SdPolyWindow::SdPolyWindow(SdPoint center, int rad) :
  radius( rad ),
  p1(center),p2(),p3(),p4()
  {

  }



//From region
SdPolyWindow::SdPolyWindow(const QPolygonF &pgn) :
  radius(-1),p1(),p2(),p3(),p4()
  {
  if( pgn.count() == 4 ) {
    p1 = pgn.at(0).toPoint();
    p2 = pgn.at(1).toPoint();
    p3 = pgn.at(2).toPoint();
    p4 = pgn.at(3).toPoint();
    mPolygon = pgn;
    }
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
  buildPolygon();
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
  buildPolygon();
  }




bool SdPolyWindow::containsPoint(SdPoint p) const
  {
  if( radius >= 0 )
    //Return as circle
    return p1.getDistanceInt(p) < radius;
  //Return as region
  return mPolygon.containsPoint( p.toPointF(), Qt::OddEvenFill );
  }



//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdPolyWindow::json(SvJsonWriter &js) const
  {
  js.jsonInt( QStringLiteral("r"), radius );
  js.jsonPoint( QStringLiteral("p1"), p1 );
  js.jsonPoint( QStringLiteral("p2"), p2 );
  js.jsonPoint( QStringLiteral("p3"), p3 );
  js.jsonPoint( QStringLiteral("p4"), p4 );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdPolyWindow::json(const SvJsonReader &js)
  {
  js.jsonInt( QStringLiteral("r"), radius );
  js.jsonPoint( QStringLiteral("p1"), p1 );
  js.jsonPoint( QStringLiteral("p2"), p2 );
  js.jsonPoint( QStringLiteral("p3"), p3 );
  js.jsonPoint( QStringLiteral("p4"), p4 );
  buildPolygon();
  }


