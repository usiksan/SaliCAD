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

class SdSegment
  {
    SdPoint p1;
    SdPoint p2;
  public:
    enum SdOrientation {
      sorNull,          //For p1 == p2
      sorVertical,      //For p1.x == p2.x
      sorHorizontal,    //For p1.y == p2.y
      sorSlashForward,  //For dx == dy
      sorSlashBackward, //For dx == -dy
      sorAny
      };

    SdSegment() : p1(), p2() {}
    SdSegment( SdPoint a, SdPoint b ) : p1(a), p2(b) {}

    SdPoint getP1() const { return p1; }
    SdPoint getP2() const { return p2; }
    void    set( SdPoint a, SdPoint b ) { p1 = a; p2 = b; }
    void    setP1( SdPoint p ) { p1 = p; }
    void    setP2( SdPoint p ) { p2 = p; }
    void          move( SdPoint offset ) { p1.move( offset ); p2.move( offset ); }
    void          moveP1( SdPoint offset ) { p1.move( offset ); }
    void          moveP2( SdPoint offset ) { p2.move( offset ); }
    SdPoint middle() const { return SdPoint( (p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2 ); }
    bool    isSectionX() const { return p1.y() == p2.y(); }
    bool    isSectionY() const { return p1.x() == p2.x(); }

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
    SdOrientation orientation() const;

    void    writeSegment( QJsonObject &obj ) const;
    void    readSegment( const QJsonObject obj );

  };

#endif // SDSEGMENT_H
