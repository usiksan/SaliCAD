/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdContext.h"


SdContext::SdContext(SdPoint grid, QPainter *painter) :
  mPainter(painter),
  mConverter(0),
  mGrid(grid),
  mSelector(0)
  {

  }

void SdContext::line(SdPoint a, SdPoint b, SdPropLine &prop)
  {

  setPen( )
  }

void SdContext::rect(SdQuadrangle q, SdPropLine &prop)
  {
  //Draw 4 edges
  line( q.p1, q.p2, prop );
  line( q.p2, q.p3, prop );
  line( q.p3, q.p4, prop );
  line( q.p4, q.p1, prop );
  }

void SdContext::region(SdPointList &points, SdPropLine &prop, bool autoClose)
  {
  if( points.count() > 1 ) {
    for( int i = 1; i < points.count(); i++ )
      line( points.at(i-1), points.at(i), prop );
    if( autoClose && points.first() != points.last() )
      line( points.first(), points.last(), prop );
    }

  }

void SdContext::dotTrase(SdPoint p)
  {
  Q_UNUSED(p)
  }

void SdContext::setPen(int width, unsigned color)
  {
  mPainter->setPen( QPen( QBrush(QRgb(color)), width ) );
  }
