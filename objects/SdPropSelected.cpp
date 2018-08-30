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
#include "SdPropSelected.h"
#include "windows/SdWCommand.h"

SdPropSelected::SdPropSelected() :
  mPropBarId(PB_DEFAULT)
  {
  clear();
  }



//Clear all properties
void SdPropSelected::clear()
  {
  mLineProp.clear();       //Properties for linear objects
  mTextProp.clear();       //Properties for textual objects
  mWireProp.clear();       //Properties for wire objects
  mSymPinProp.clear();     //Properties for symbol pin object
  mPartPinProp.clear();    //Properties for part pin object
  mSymImpProp.clear();     //Properties for symbol implement in sheet
  mPartImpProp.clear();    //Properties for part implement in pcb
  mRoadProp.clear();       //Properties for road
  mPolygonProp.clear();    //Properties for polygon
//    SdViaProp    viaProp;        //Свойства переходных отверстий
  mEnterType = 0;
  mWireName.clear();       //Wire name
//    DTraseRules traseRules;     //Правила трассировки

  mFilledPropMask = 0; //Mask for filled properties types
  }




int SdPropSelected::getPropBarId()
  {
  //Test if current prop bar accorded to prop filled mask
  // if true then return current prop bar
  switch( mPropBarId ) {
    case PB_LINEAR:
      if( mFilledPropMask & spsLineProp ) return PB_LINEAR;
      break;
    case PB_TEXT :
      if( mFilledPropMask & spsTextProp ) return PB_TEXT;
      break;
    case PB_WIRE :
      if( mFilledPropMask & spsWireProp ) return PB_WIRE;
      break;
    case PB_SYM_PIN :
      if( mFilledPropMask & spsSymPinProp ) return PB_SYM_PIN;
      break;
    case PB_PART_PIN :
      if( mFilledPropMask & spsPartPinProp ) return PB_PART_PIN;
      break;
    case PB_SYM_IMP :
      if( mFilledPropMask & spsSymImpProp ) return PB_SYM_IMP;
      break;
    case PB_PART_IMP :
      if( mFilledPropMask & spsPartImpProp ) return PB_PART_IMP;
      break;
    case PB_ROAD :
      if( mFilledPropMask & spsRoadProp ) return PB_ROAD;
      break;
    case PB_POLYGON :
      if( mFilledPropMask & spsPolygonProp ) return PB_POLYGON;
      break;
    }
  //No current bar in prop, find new prop bar
  if( mFilledPropMask & spsLineProp )
    return mPropBarId = PB_LINEAR;
  if( mFilledPropMask & spsTextProp )
    return mPropBarId = PB_TEXT;
  if( mFilledPropMask & spsWireProp )
    return mPropBarId = PB_WIRE;
  if( mFilledPropMask & spsSymPinProp )
    return mPropBarId = PB_SYM_PIN;
  if( mFilledPropMask & spsPartPinProp )
    return mPropBarId = PB_PART_PIN;
  if( mFilledPropMask & spsSymImpProp )
    return mPropBarId = PB_SYM_IMP;
  if( mFilledPropMask & spsPartImpProp )
    return mPropBarId = PB_PART_IMP;
  if( mFilledPropMask & spsRoadProp )
    return mPropBarId = PB_ROAD;
  if( mFilledPropMask & spsPolygonProp )
    return mPropBarId = PB_POLYGON;

  //Return previous prop bar
  return mPropBarId;
  }

