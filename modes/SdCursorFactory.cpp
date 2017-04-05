#include "SdCursorFactory.h"
#include <QString>

QCursor loadWindowsCursor( const QString fname );

QCursor loadCursor(int cursorId)
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
#define CUR_HAND        112
#define CUR_HYPER       113
#define CUR_IDENT       114
#define CUR_LINE        115
#define CUR_MOVE        116
#define CUR_NETLIST     117
#define CUR_ORG         118
#define CUR_PIN         119
#define CUR_PINNUM      120
#define CUR_PINNAM      121
#define CUR_PLACE       122
#define CUR_POINT       123
#define CUR_RECT        124
#define CUR_REGION      125
#define CUR_ROTATE      126
#define CUR_RULLER      127
#define CUR_SEL         128
#define CUR_SIZE_LINEAR 129
#define CUR_SIZE_RADIAL 130
#define CUR_STOP        131
#define CUR_TAKE        132
#define CUR_TAKE_COPY   133
#define CUR_TEXT        134
#define CUR_UCOM        135
#define CUR_VERTEX      136
#define CUR_VIA_ADD     137
#define CUR_VIA_DEL     138
#define CUR_WHAND       139
#define CUR_WIRE        140
#define CUR_WMOVE       141
#define CUR_WREADY      142
#define CUR_WSELECT     143
#define CUR_ZOOM_IN     144
#define CUR_ZOOM_OUT    145
#define CUR_ZOOM_WIN    146

    }
  }
