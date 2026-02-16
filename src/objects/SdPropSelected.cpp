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
  mViaProp.clear();        //Properties for via [Свойства переходных отверстий]
  mEnterType = 0;
  mWireName.clear();       //Wire name

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
  if( mFilledPropMask & spsSymPinProp )
    return mPropBarId = PB_SYM_PIN;
  if( mFilledPropMask & spsPartPinProp )
    return mPropBarId = PB_PART_PIN;
  if( mFilledPropMask & spsLineProp )
    return mPropBarId = PB_LINEAR;
  if( mFilledPropMask & spsTextProp )
    return mPropBarId = PB_TEXT;
  if( mFilledPropMask & spsWireProp )
    return mPropBarId = PB_WIRE;
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




//Layer present in many properties. If selected some different objects then
//properties bar will be only for one class of objects. But layer
//must be changed for all selected objects
//With this function we change layer for all properties group
void SdPropSelected::setLayer(const SdPropLayer &layer)
  {
  mLineProp.get<&SdPropLine::mLayer>().reset( layer );       //Properties for linear objects
  mTextProp.get<&SdPropText::mLayer>().reset( layer );       //Properties for textual objects
  mWireProp.get<&SdPropLine::mLayer>().reset( layer );       //Properties for wire objects
  mSymPinProp.get<&SdPropSymPin::mLayer>().reset( layer );     //Properties for symbol pin object
  mPartPinProp.get<&SdPropPartPin::mLayer>().reset( layer );    //Properties for part pin object
  }




//When selected different classes of objects then layer state is
//in different preperties group. With this we union all this group
void SdPropSelected::unionLayer()
  {
  SdPropMulty<SdPropLayer> layer;
  layer.clear();
  layer.append( mLineProp.get<&SdPropLine::mLayer>() );
  layer.append( mTextProp.get<&SdPropText::mLayer>() );
  layer.append( mWireProp.get<&SdPropLine::mLayer>() );
  layer.append( mSymPinProp.get<&SdPropSymPin::mLayer>() );
  layer.append( mPartPinProp.get<&SdPropPartPin::mLayer>() );

  mLineProp.get<&SdPropLine::mLayer>() = layer;       //Properties for linear objects
  mTextProp.get<&SdPropText::mLayer>() = layer;       //Properties for textual objects
  mWireProp.get<&SdPropLine::mLayer>() = layer;       //Properties for wire objects
  mSymPinProp.get<&SdPropSymPin::mLayer>() = layer;     //Properties for symbol pin object
  mPartPinProp.get<&SdPropPartPin::mLayer>() = layer;    //Properties for part pin object
  }

