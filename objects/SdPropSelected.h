/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties collection for selection mode
*/
#ifndef SDPROPSELECTED_H
#define SDPROPSELECTED_H

#include "SdProp.h"

//Masks for filled properties types
#define spsLineProp    0x001
#define spsTextProp    0x002
#define spsWireProp    0x004
#define spsSymPinProp  0x008
#define spsPartPinProp 0x010
#define spsSymImpProp  0x020
#define spsPartImpProp 0x040
#define spsRoadProp    0x080
#define spsPolygonProp 0x100


struct SdPropSelected
  {
    SdPropLine    mLineProp;       //Properties for linear objects
    SdPropText    mTextProp;       //Properties for textual objects
    SdPropLine    mWireProp;       //Properties for wire objects
    SdPropSymPin  mSymPinProp;     //Properties for symbol pin object
    SdPropPartPin mPartPinProp;    //Properties for part pin object
    SdPropSymImp  mSymImpProp;     //Properties for symbol implement in sheet
    SdPropPartImp mPartImpProp;    //Properties for part implement in pcb
    SdPropRoad    mRoadProp;       //Properties for road
    SdPropPolygon mPolygonProp;    //Properties for polygon
    SdPropVia     mViaProp;        //Properties for via [Свойства переходных отверстий]

    int           mEnterType;
    SdPropString  mWireName;       //Wire name

    int           mFilledPropMask; //Mask for filled properties types
    int           mPropBarId;      //Current bar id

    SdPropSelected();

    //Clear all properties
    void clear();

    //Get prop bar id for first filled prop mask
    int  getPropBarId();

  };

#endif // SDPROPSELECTED_H
