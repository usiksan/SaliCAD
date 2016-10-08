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


class SdSnapInfo
  {
  public:
    SdSnapInfo();
  };

#endif // SDSNAPINFO_H
