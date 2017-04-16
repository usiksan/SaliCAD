/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdConverter.h"
#include "SdContext.h"


SdConverter::SdConverter(SdContext *context) :
  mNext(0),
  mContext(context),
  mPairLayer(false)
  {
  }


SdConverter::~SdConverter()
  {
  mContext->removeConverter( this );
  }



//Convert layer flag
bool SdConverter::getFullPairLayer()
  {
  if( mNext )
    return mPairLayer ? !mNext->getFullPairLayer() : mNext->getFullPairLayer();
  return  mPairLayer;
  }




//Calculate and return full transformation matrix
QTransform SdConverter::getFullMatrix()
  {
  QTransform m = getMatrix();
  for( SdConverter *c = mNext; c; c = c->mNext )
    m *= c->getMatrix();
  return m;
  }



double SdConverter::getScale() const
  {
  if( mNext )
    return mNext->getScale();
  return 1.0;
  }

