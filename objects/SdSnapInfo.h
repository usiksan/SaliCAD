/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDSNAPINFO_H
#define SDSNAPINFO_H

#include "SdPoint.h"
#include "SdAngle.h"

//Маска разумной привязки
const int
  snapEndPoint          = 0x0001,
  snapMidPoint          = 0x0002,
  snapIntersection      = 0x0004,
  snapApparentIntersect = 0x0008,
  snapExtension         = 0x0010,
  snapCenter            = 0x0020,
  snapQuadrant          = 0x0040,
  snapTangent           = 0x0080,
  snapPerpendecular     = 0x0100,
  snapParallel          = 0x0200,
  snapNetPoint          = 0x0400,
  snapPrev              = 0x0800,
  snapAll               = snapEndPoint | snapMidPoint | snapIntersection |
                          snapApparentIntersect | snapExtension | snapCenter |
                          snapQuadrant | snapTangent | snapPerpendecular |
                          snapParallel | snapNetPoint,
  snapNextPin           = 0x1000,   //Следующий вывод
  snapNearestPin        = 0x2000,   //Ближайший вывод
  snapNearestNet        = 0x4000,   //Ближайшая цепь
  snapCommon            = 0x8000,
  snapNearestNetPin     = 0x10000,  //Ближайший вывод принадлежащий цепи
  snapNearestNetNet     = 0x20000;  //Ближайшая точка принадлежащая цепи и на дорожке (полигоне)

//Значения для вычисления специальных точек
const int
  dsifExSour = 0x000001,   //Исключить из просмотра исходную точку
  dsifExExcl = 0x000002    //Исключить из просмотра точку exclude
  ;

class SdContainer;

struct SdSnapInfo
  {
    SdPoint         mSour;     //Исходная точка
    SdPoint         mDest;     //Получившаяся точка
    SdPoint         mExclude;  //Точка, которую нужно исключить из просмотра
    SdAngle         mDir;      //Направление поиска
    int             mSnapMask; //Маска, которую нужно проверить
    int             mDestMask; //Маска сработавшая для точки dest
    double          mDistance; //Текущее расстояние от точки sour до точки dest
    int             mFlag;     //Флаг управления поиском (см. выше)
    QString         mNetName;  //Имя цепи для поиска
    int             mSide;     //Сторона, на которой осуществлять поиск

    SdSnapInfo();

    bool test( SdPoint p, int mask );
    void calculate( SdContainer *object );
    bool match( int t ) const { return (mSnapMask & t) != 0; }
    void reset( bool resetDest = true ); //Сброс для следующего поиска
  };

#endif // SDSNAPINFO_H
