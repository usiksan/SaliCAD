/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDSEGMENT_H
#define SDSEGMENT_H

#include "SdPoint.h"

class SdRect;

class SdSegment
  {
    SdPoint p1;
    SdPoint p2;
  public:

    SdSegment() : p1(), p2() {}
    SdSegment( SdPoint a, SdPoint b ) : p1(a), p2(b) {}

    //End points
    SdPoint       getP1() const { return p1; }
    SdPoint       getP2() const { return p2; }

    //Set end points
    void          set( SdPoint a, SdPoint b ) { p1 = a; p2 = b; }
    void          setP1( SdPoint p ) { p1 = p; }
    void          setP2( SdPoint p ) { p2 = p; }

    //Move end points
    void          move( SdPoint offset ) { p1.move( offset ); p2.move( offset ); }
    void          moveP1( SdPoint offset ) { p1.move( offset ); }
    void          moveP2( SdPoint offset ) { p2.move( offset ); }

    //End point pointers
    SdPoint      *ptrP1() { return &p1; }
    SdPoint      *ptrP2() { return &p2; }

    //Middle point of segment
    SdPoint       middle() const { return SdPoint( (p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2 ); }

    //Vertex point with 45 degree step
    SdPoint       vertex45() const;

    //Return true if segment is horizontal
    bool          isSectionX() const { return p1.y() == p2.y(); }

    //Return true if segment is vertical
    bool          isSectionY() const { return p1.x() == p2.x(); }

    //Return true if segment with zero lenght
    bool          isZero() const { return p1 == p2; }

    bool    isCross( SdPoint a, SdPoint b , SdPoint *out = nullptr ) const;
    bool    isCross( SdSegment s ) const { return isCross( s.p1, s.p2 ); }
    bool    isContinue( SdSegment s ) const;
    bool    isPointOn( SdPoint p ) const;
    int     vectorMultiplication( SdPoint a, SdPoint b ) const;
    void    calcLineK( double &A, double &B, double &C ) const;
    double  lineDistance( SdPoint p ) const;
    SdPoint getLineNearest( SdPoint sour ) const;
    bool    isOneSideLine( const SdSegment &s ) const;

    //Calculation segment orientation
    SdOrientation  orientation() const;

    //Angle of segment
    double         getAngleDegree() const { return p2.getAngleDegree(p1); }

    //Angle of segment as properties
    SdPropAngle    getAngle() const { return p2.getAngle(p1); }

    //Distance between two ends
    double         getLenght() const { return p1.getDistance(p2); }

    //Distance between two ends as int
    int            getLenghtInt() const { return p1.getDistanceInt(p2); }

    //Square distance between two ends
    double         getSquareLenght() const { return p1.getSquareDistance(p2); }

    //Return over rect
    SdRect         getOverRect() const;


    //Write-read as segment. Coord values write immediately to obj
    void           writeSegment( QJsonObject &obj ) const;
    void           readSegment( const QJsonObject obj );

  };

#endif // SDSEGMENT_H
