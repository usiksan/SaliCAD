/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdSnapInfo - info structure for snap system
  Snap system allow link to appropriate points.
*/

#ifndef SDSNAPINFO_H
#define SDSNAPINFO_H

#include "SdPoint.h"
#include "SdPropAngle.h"
#include "SdStratum.h"

//Маска разумной привязки
#define snapEndPoint          0x00000001
#define snapMidPoint          0x00000002
#define snapIntersection      0x00000004
#define snapApparentIntersect 0x00000008
#define snapExtension         0x00000010
#define snapCenter            0x00000020
#define snapQuadrant          0x00000040
#define snapTangent           0x00000080
#define snapPerpendecular     0x00000100
#define snapParallel          0x00000200
#define snapNetPoint          0x00000400
#define snapPrev              0x00000800
#define snapAll               (snapEndPoint | snapMidPoint | snapIntersection | \
                          snapApparentIntersect | snapExtension | snapCenter | \
                          snapQuadrant | snapTangent | snapPerpendecular | \
                          snapParallel | snapNetPoint)
#define snapNextPin           0x00001000   //Следующий вывод
#define snapNearestPin        0x00002000   //Ближайший вывод
#define snapNearestNet        0x00004000   //Ближайшая цепь
#define snapCommon            0x00008000
#define snapNearestNetPin     0x00010000  //Ближайший вывод принадлежащий цепи
#define snapNearestNetNet     0x00020000  //Ближайшая точка принадлежащая цепи и на дорожке (полигоне)
#define snapFull              0x0fffffff

//Значения для вычисления специальных точек
#define snapExcludeSour       0x10000000   //Исключить из просмотра исходную точку
#define snapExcludeExcl       0x20000000   //Исключить из просмотра точку exclude


class SdContainer;

typedef quint32 SdSnapMask;

struct SdSnapInfo
  {
    SdPoint         mSour;     //Исходная точка
    SdPoint         mDest;     //Получившаяся точка
    SdPoint         mExclude;  //Точка, которую нужно исключить из просмотра
    SdPropAngle         mDir;      //Направление поиска
    SdSnapMask      mSnapMask; //Маска, которую нужно проверить
    SdSnapMask      mDestMask; //Маска сработавшая для точки dest
    double          mDistance; //Текущее расстояние от точки sour до точки dest
    QString         mNetName;  //Имя цепи для поиска
    SdStratum       mStratum;  //Stratum for tracing

    SdSnapInfo();

    bool test( SdPoint p, SdSnapMask mask );
    void calculate( SdContainer *object );
    bool match( SdSnapMask t ) const { return (mSnapMask & t) != 0; }
    void reset( bool resetDest = true ); //Сброс для следующего поиска
  };

#endif // SDSNAPINFO_H
