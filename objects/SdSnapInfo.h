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
#include "SdAngle.h"

//Маска разумной привязки
#define snapEndPoint          0x0001
#define snapMidPoint          0x0002
#define snapIntersection      0x0004
#define snapApparentIntersect 0x0008
#define snapExtension         0x0010
#define snapCenter            0x0020
#define snapQuadrant          0x0040
#define snapTangent           0x0080
#define snapPerpendecular     0x0100
#define snapParallel          0x0200
#define snapNetPoint          0x0400
#define snapPrev              0x0800
#define snapAll               (snapEndPoint | snapMidPoint | snapIntersection | \
                          snapApparentIntersect | snapExtension | snapCenter | \
                          snapQuadrant | snapTangent | snapPerpendecular | \
                          snapParallel | snapNetPoint)
#define snapNextPin           0x1000   //Следующий вывод
#define snapNearestPin        0x2000   //Ближайший вывод
#define snapNearestNet        0x4000   //Ближайшая цепь
#define snapCommon            0x8000
#define snapNearestNetPin     0x10000  //Ближайший вывод принадлежащий цепи
#define snapNearestNetNet     0x20000  //Ближайшая точка принадлежащая цепи и на дорожке (полигоне)
#define snapFull              0xffffffff

//Значения для вычисления специальных точек
#define dsifExSour            0x000001   //Исключить из просмотра исходную точку
#define dsifExExcl            0x000002    //Исключить из просмотра точку exclude


class SdContainer;

typedef quint32 SdSnapMask;

struct SdSnapInfo
  {
    SdPoint         mSour;     //Исходная точка
    SdPoint         mDest;     //Получившаяся точка
    SdPoint         mExclude;  //Точка, которую нужно исключить из просмотра
    SdAngle         mDir;      //Направление поиска
    SdSnapMask      mSnapMask; //Маска, которую нужно проверить
    SdSnapMask      mDestMask; //Маска сработавшая для точки dest
    double          mDistance; //Текущее расстояние от точки sour до точки dest
    int             mFlag;     //Флаг управления поиском (см. выше)
    QString         mNetName;  //Имя цепи для поиска
    int             mSide;     //Сторона, на которой осуществлять поиск

    SdSnapInfo();

    bool test( SdPoint p, SdSnapMask mask );
    void calculate( SdContainer *object );
    bool match( SdSnapMask t ) const { return (mSnapMask & t) != 0; }
    void reset( bool resetDest = true ); //Сброс для следующего поиска
  };

#endif // SDSNAPINFO_H
