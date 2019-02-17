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
#ifndef SDCURSORFACTORY_H
#define SDCURSORFACTORY_H

#include <QCursor>

#define CUR_ARC         100
#define CUR_AREA        101
#define CUR_ARROW       102
#define CUR_BLACK       103
#define CUR_BUS         104
#define CUR_CELL        105
#define CUR_CIRCLE      106
#define CUR_COPY        107
#define CUR_CUT         108
#define CUR_DRAGC       109
#define CUR_DRAGM       110
#define CUR_GROUP       111
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
#define CUR_COMP        147


QCursor loadCursor( int cursorId );

#endif // SDCURSORFACTORY_H
