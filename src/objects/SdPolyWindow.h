/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Window in polygon.
  Window may be a quadrangle or circle. All others - are combination of this.
*/

#ifndef SDPOLYWINDOW_H
#define SDPOLYWINDOW_H

#include "SdPoint.h"
#include "SdCircle.h"

#include <QPolygonF>

class SdPolyWindow
  {
    int     radius;
    SdPoint p1;
    SdPoint p2;
    SdPoint p3;
    SdPoint p4;

    //Not saved
    QPolygonF mPolygon;
    void      buildPolygon();
  public:
    SdPolyWindow();

    //From circle
    SdPolyWindow( SdCircle circle, int gap );
    SdPolyWindow(SdPoint center, int rad );

    //From region
    SdPolyWindow( const QPolygonF &pgn );

    //From rect
    SdPolyWindow( SdRect r, int gap );

    //From road
    SdPolyWindow( SdPoint a, SdPoint b, int gap );

    int         getRadius() const { return radius; }
    SdPoint     getP1() const { return p1; }
    SdPoint     getP2() const { return p2; }
    SdPoint     getP3() const { return p3; }
    SdPoint     getP4() const { return p4; }

    QPolygonF   polygon() const { return mPolygon; }

    bool        containsPoint( SdPoint p ) const;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    void        json( SvJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    void        json( const SvJsonReader &js);
  };

#endif // SDPOLYWINDOW_H
