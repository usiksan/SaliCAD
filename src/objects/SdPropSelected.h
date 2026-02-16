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
#include "objects/SdPropMulty.h"

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

using SdPropComposerLine = SdPropComposer<SdPropLine, &SdPropLine::mLayer, &SdPropLine::mType, &SdPropLine::mWidth>;
using SdPropComposerText = SdPropComposer<SdPropText, &SdPropText::mDir, &SdPropText::mFont, &SdPropText::mHorz,
                                        &SdPropText::mLayer, &SdPropText::mMirror, &SdPropText::mSize, &SdPropText::mVert>;
using SdPropComposerSymPin = SdPropComposer<SdPropSymPin, &SdPropSymPin::mLayer, &SdPropSymPin::mPinType>;
using SdPropComposerPartPin = SdPropComposer<SdPropPartPin, &SdPropPartPin::mLayer, &SdPropPartPin::mPinType, &SdPropPartPin::mSide>;
using SdPropComposerSymImp = SdPropComposer<SdPropSymImp, &SdPropSymImp::mAngle, &SdPropSymImp::mMirror>;
using SdPropComposerPartImp = SdPropComposer<SdPropPartImp, &SdPropPartImp::mAngle, &SdPropPartImp::mSide>;
using SdPropComposerRoad = SdPropComposer<SdPropRoad, &SdPropRoad::mNetName, &SdPropRoad::mStratum, &SdPropRoad::mWidth>;
using SdPropComposerPolygon = SdPropComposer<SdPropPolygon, &SdPropPolygon::mGap, &SdPropPolygon::mNetName, &SdPropPolygon::mStratum>;
using SdPropComposerVia = SdPropComposer<SdPropVia, &SdPropVia::mNetName, &SdPropVia::mPadType, &SdPropVia::mStratum>;
using SdPropMultyString = SdPropMulty<SdPropString>;

struct SdPropSelected
  {
    SdPropComposerLine    mLineProp;       //Properties for linear objects
    SdPropComposerText    mTextProp;       //Properties for textual objects
    SdPropComposerLine    mWireProp;       //Properties for wire objects
    SdPropComposerSymPin  mSymPinProp;     //Properties for symbol pin object
    SdPropComposerPartPin mPartPinProp;    //Properties for part pin object
    SdPropComposerSymImp  mSymImpProp;     //Properties for symbol implement in sheet
    SdPropComposerPartImp mPartImpProp;    //Properties for part implement in pcb
    SdPropComposerRoad    mRoadProp;       //Properties for road
    SdPropComposerPolygon mPolygonProp;    //Properties for polygon
    SdPropComposerVia     mViaProp;        //Properties for via [Свойства переходных отверстий]

    int                   mEnterType;
    SdPropMultyString     mWireName;       //Wire name

    int                   mFilledPropMask; //Mask for filled properties types
    int                   mPropBarId;      //Current bar id

    SdPropSelected();

    //Clear all properties
    void clear();

    //Get prop bar id for first filled prop mask
    int  getPropBarId();

    //Layer present in many properties. If selected some different objects then
    //properties bar will be only for one class of objects. But layer
    //must be changed for all selected objects
    //With this function we change layer for all properties group
    void setLayer(const SdPropLayer &layer );

    //When selected different classes of objects then layer state is
    //in different preperties group. With this we union all this group
    void unionLayer();
  };

#endif // SDPROPSELECTED_H
