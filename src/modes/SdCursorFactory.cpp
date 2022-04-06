/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Cursor factory. Contains cursor images and retrive them on demand.
*/
#include "SdCursorFactory.h"
#include <QString>
#include <QMap>
#include <QImage>
#include <QPixmap>

QCursor loadWindowsCursor( const QString fname );


static QCursor createCursor( int cursorId )
  {
  switch( cursorId ) {
    case CUR_ARC         : return loadWindowsCursor( QString(":/pic/arc.cur") );
    case CUR_AREA        : return loadWindowsCursor( QString(":/pic/area.cur") );
    case CUR_ARROW       : return loadWindowsCursor( QString(":/pic/arrow.cur") );
    case CUR_BLACK       : return loadWindowsCursor( QString(":/pic/black.cur") );
    case CUR_BUS         : return loadWindowsCursor( QString(":/pic/bus.cur") );
    case CUR_CELL        : return loadWindowsCursor( QString(":/pic/cell.cur") );
    case CUR_CIRCLE      : return loadWindowsCursor( QString(":/pic/circle.cur") );
    case CUR_COPY        : return loadWindowsCursor( QString(":/pic/copy.cur") );
    case CUR_CUT         : return loadWindowsCursor( QString(":/pic/cut.cur") );
    case CUR_DRAGC       : return loadWindowsCursor( QString(":/pic/dragc.cur") );
    case CUR_DRAGM       : return loadWindowsCursor( QString(":/pic/dragm.cur") );
    case CUR_GROUP       : return loadWindowsCursor( QString(":/pic/group.cur") );
    case CUR_HAND        : return loadWindowsCursor( QString(":/pic/hand.cur") );
    case CUR_HYPER       : return loadWindowsCursor( QString(":/pic/hyper.cur") );
    case CUR_IDENT       : return loadWindowsCursor( QString(":/pic/ident.cur") );
    case CUR_LINE        : return loadWindowsCursor( QString(":/pic/line.cur") );
    case CUR_MOVE        : return loadWindowsCursor( QString(":/pic/move.cur") );
    case CUR_NETLIST     : return loadWindowsCursor( QString(":/pic/netlist.cur") );
    case CUR_ORG         : return loadWindowsCursor( QString(":/pic/org.cur") );
    case CUR_PIN         : return loadWindowsCursor( QString(":/pic/pin.cur") );
    case CUR_PINNUM      : return loadWindowsCursor( QString(":/pic/pinnum.cur") );
    case CUR_PINNAM      : return loadWindowsCursor( QString(":/pic/pinnam.cur") );
    case CUR_PLACE       : return loadWindowsCursor( QString(":/pic/place.cur") );
    case CUR_POINT       : return loadWindowsCursor( QString(":/pic/point.cur") );
    case CUR_RECT        : return loadWindowsCursor( QString(":/pic/rect.cur") );
    case CUR_REGION      : return loadWindowsCursor( QString(":/pic/region.cur") );
    case CUR_ROTATE      : return loadWindowsCursor( QString(":/pic/rotate.cur") );
    case CUR_RULLER      : return loadWindowsCursor( QString(":/pic/ruler.cur") );
    case CUR_SEL         : return loadWindowsCursor( QString(":/pic/sel.cur") );
    case CUR_SIZE_LINEAR : return loadWindowsCursor( QString(":/pic/sizeLinear.cur") );
    case CUR_SIZE_RADIAL : return loadWindowsCursor( QString(":/pic/sizeRadial.cur") );
    case CUR_STOP        : return loadWindowsCursor( QString(":/pic/stop.cur") );
    case CUR_TAKE        : return loadWindowsCursor( QString(":/pic/take.cur") );
    case CUR_TAKE_COPY   : return loadWindowsCursor( QString(":/pic/copy.cur") );
    case CUR_TEXT        : return loadWindowsCursor( QString(":/pic/text.cur") );
    case CUR_UCOM        : return loadWindowsCursor( QString(":/pic/ucom.cur") );
    case CUR_VERTEX      : return loadWindowsCursor( QString(":/pic/vertex.cur") );
    case CUR_VIA_ADD     : return loadWindowsCursor( QString(":/pic/viaAdd.cur") );
    case CUR_VIA_DEL     : return loadWindowsCursor( QString(":/pic/viaDel.cur") );
    case CUR_WHAND       : return loadWindowsCursor( QString(":/pic/whand.cur") );
    case CUR_WIRE        : return loadWindowsCursor( QString(":/pic/wire.cur") );
    case CUR_WMOVE       : return loadWindowsCursor( QString(":/pic/wmove.cur") );
    case CUR_WREADY      : return loadWindowsCursor( QString(":/pic/wready.cur") );
    case CUR_WSELECT     : return loadWindowsCursor( QString(":/pic/wselect.cur") );
    case CUR_ZOOM_IN     : return loadWindowsCursor( QString(":/pic/zoomin.cur") );
    case CUR_ZOOM_OUT    : return loadWindowsCursor( QString(":/pic/zoomout.cur") );
    case CUR_ZOOM_WIN    : return loadWindowsCursor( QString(":/pic/zoomwin.cur") );
    case CUR_COMP        : return QCursor( QPixmap(QStringLiteral(":/pic/cursorComp.png")), 4, 4 );
    case CUR_SCRIPT      : return QCursor( QPixmap(QStringLiteral(":/pic/cursorScript.png")), 5, 5 );
    }
  return QCursor();
  }




QCursor loadCursor(int cursorId)
  {
  static QMap<int,QCursor> cursorMap;
  if( cursorMap.contains(cursorId) )
    return cursorMap.value(cursorId);
  cursorMap.insert( cursorId, createCursor(cursorId) );
  return cursorMap.value(cursorId);
  }
