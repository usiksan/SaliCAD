/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  MasterPart used to build part with part masters

  Contains operations for append graphics and pins to part when creation.
*/
#ifndef SDMASTERPART_H
#define SDMASTERPART_H

#include "objects/SdProjectItem.h"

struct SdMasterPart
  {
    SdProjectItem *mItem;
    SdPropLine     mLineProp;       //All graph objects append by default to "component" layer with 0-width
    SdPropPartPin  mPinProp;        //All pins
    SdPropText     mIdentProp;
    SdPropText     mValueProp;
    SdPropText     mPinNumberProp;
    SdPropText     mPinNameProp;


    //Add common graphics to part
    void addLine( int x1, int y1, int x2, int y2 );
    void addRect( int x1, int y1, int x2, int y2 );
    void addFRect( int x1, int y1, int x2, int y2 );
    void addCircle( int cx, int cy, int r );

    //Identifier append to "id" layer
    void setId(SdPoint p, int size = 1000 );

    //Value placeholder append to "value" layer
    void setValue(SdPoint p, int size = 1000 );

    //Pin append to "pin" layer
    void setupSmdPin();
    void setupThrouPin();
    void addPin( SdPoint org, const QString type, SdPoint pinNumberOrg, const QString pinNumber, SdPoint pinNameOrg );

    SdMasterPart(SdProjectItem *item);
  };

#endif // SDMASTERPART_H
