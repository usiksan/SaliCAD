/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPOLYWINDOWLIST_H
#define SDPOLYWINDOWLIST_H

#include "SdPolyWindow.h"
#include "SdPointList.h"

#include <QList>

class SdPolyWindowList : public QList<SdPolyWindow>
  {
    SdPointList *mPolygon; //appropriate polygon
  public:
    SdPolyWindowList() : mPolygon(nullptr) {}

    //Reset windows list
    void         reset( SdPointList *poly );

    //Return polygon which window list
    SdPointList *polygon() const { return mPolygon; }

    //Appending service
    void         appendRegion( const QPolygonF &pgn );
    void         appendCircle( SdPoint center, int radius );

    //Test point
    bool         containsPoint( SdPoint p ) const;

    QJsonArray   write() const;
    void         write( const QString name, QJsonObject &obj ) const;

    void         read( const QJsonArray array );
    void         read( const QString name, const QJsonObject obj );
  };

#endif // SDPOLYWINDOWLIST_H
