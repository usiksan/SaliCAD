/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Barrier formed by tracing object with assiciated net name
*/

#include "SdBarrier.h"
#include <QLineF>

//Check if point p is inside any barrier and return true or false if not
bool sdIsBarrierListContains(const SdBarrierList &list, QString netName, SdPoint p)
  {
  QPointF pf(p.toPointF());
  for( const SdBarrier &bar : list )
    if( bar.mNetName != netName && bar.mPolygon.containsPoint(pf, Qt::OddEvenFill) )
      return true;
  return false;
  }






//Try apply road to barrier list. If no road intersection with barrier then return second road point
//else return available second road point with which road is not intersected with barrier
SdPoint sdCheckRoadOnBarrierList(const SdBarrierList &barrierList, SdPoint p1, SdPoint p2, QString netName)
  {
  //If start point inside barriers then no possible trace
  if( sdIsBarrierListContains( barrierList, netName, p1 ) )
    return p1;

  //Source line
  QLineF sf(p1.toPointF(),p2.toPointF());
  //Point for save intersection
  QPointF d;
  for( const SdBarrier &b : barrierList )
    if( b.mNetName != netName ) {
      //For each edge of polygon we check intersection with our sf line
      for( int i = 1; i < b.mPolygon.count(); i++ ) {
        //Take next edge of polygon
        QLineF df(b.mPolygon.at(i-1),b.mPolygon.at(i));
        //If lines intersected then update sf
        if( df.intersects( sf, &d ) == QLineF::BoundedIntersection ) {
          p2 = d.toPoint();
          sf.setP2( d );
          }
        }
      //Complete with final edge
      QLineF df(b.mPolygon.first(), b.mPolygon.last() );
      //If lines intersected then update sf
      if( df.intersects( sf, &d ) == QLineF::BoundedIntersection ) {
        p2 = d.toPoint();
        sf.setP2( d );
        }
      }
  return p2;
  }
