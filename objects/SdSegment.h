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
    SdSegment() : p1(), p2() {}
    SdSegment( SdPoint a, SdPoint b ) : p1(a), p2(b) {}

    SdPoint getP1() const { return p1; }
    SdPoint getP2() const { return p2; }
    void    set( SdPoint a, SdPoint b ) { p1 = a; p2 = b; }
    void    setP1( SdPoint p ) { p1 = p; }
    void    setP2( SdPoint p ) { p2 = p; }
    void    move( SdPoint offset ) { p1.move( offset ); p2.move( offset ); }
    SdPoint middle() const { return SdPoint( (p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2 ); }

    bool    isCross( SdPoint a, SdPoint b , SdPoint *out = 0 ) const;
    bool    isCross( SdSegment s ) const { return isCross( s.p1, s.p2 ); }
    bool    isPointOn( SdPoint p ) const;
    int     vectorMultiplication( SdPoint a, SdPoint b ) const;
  };

#endif // SDSEGMENT_H
