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
#define snapEndPoint          0x00000001u
#define snapMidPoint          0x00000002u
#define snapIntersection      0x00000004u
#define snapApparentIntersect 0x00000008u
#define snapExtension         0x00000010u
#define snapCenter            0x00000020u
#define snapQuadrant          0x00000040u
#define snapTangent           0x00000080u
#define snapPerpendecular     0x00000100u
#define snapParallel          0x00000200u
#define snapNetPoint          0x00000400u
#define snapPrev              0x00000800u
#define snapAll               (snapEndPoint | snapMidPoint | snapIntersection | \
                          snapApparentIntersect | snapExtension | snapCenter | \
                          snapQuadrant | snapTangent | snapPerpendecular | \
                          snapParallel | snapNetPoint)
#define snapNextPin           0x00001000u   //Следующий вывод
#define snapNearestPin        0x00002000u   //Ближайший вывод
#define snapNearestNet        0x00004000u   //Ближайшая цепь
#define snapCommon            0x00008000u
#define snapNearestNetPin     0x00010000u  //Ближайший вывод принадлежащий цепи
#define snapNearestNetNet     0x00020000u  //Ближайшая точка принадлежащая цепи и на дорожке (полигоне)
#define snapFull              0x0fffffffu

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
    SdPropAngle     mDir;      //Направление поиска
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
