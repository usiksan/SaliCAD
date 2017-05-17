/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDGRAPHWIRINGWIRE_H
#define SDGRAPHWIRINGWIRE_H

#include "SdGraphWiring.h"
#include "SdPropLine.h"
#include "SdPoint.h"

#define SD_TYPE_WIRE "Wire"

class SdContainerSheetNet;

class SdGraphWiringWire : public SdGraphWiring
  {
    SdPoint    a,b;       //Wire segment
    SdPropLine mProp;     //Wire drawing properties
    bool       aDot,bDot; //Dots present flag

    const int vdNone = 0, vdX = 1, vdY = 2;
    int        mDir;
    bool       mFix;
  public:
    SdGraphWiringWire();

    //Convert parent to net
    SdContainerSheetNet*  getNet() const;

    //Get wire name
    QString               getWireName() const;

//    DPoint GetA() const { return info.a; }
//    DPoint GetB() const { return info.b; }
//    bool   GetDotA() const { return info.aDot; }
//    bool   GetDotB() const { return info.bDot; }
//    void   AccumLinked( DPoint a, DPoint b, DSelectorPic &sel );
//    void   Union( DWirePic *segment );
    bool   isPointOnSection( SdPoint p ) const { return p.isOnSegment( a, b ); }
//    void   Utilise();
//    void   SetDotPoint();
  protected:
//    void CalcVertexPoints( DPoint &p1, DPoint &p2, DPoint gridSize );
//    void Exchange();
//    void Fragmentation( DPoint p, DSelectorPic &sel );
//    bool GetNeedDot( DPoint p );
  };

#endif // SDGRAPHWIRINGWIRE_H
