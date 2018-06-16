/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object with linear type (line, rect, circle and so).
*/
#include "SdGraphLinear.h"

SdGraphLinear::SdGraphLinear() :
  SdGraph()
  {

  }

SdGraphLinear::SdGraphLinear(const SdPropLine &propLine) :
  SdGraph()
  {
  mProp = propLine;
  }




void SdGraphLinear::cloneFrom(const SdObject *src)
  {
  SdGraph::cloneFrom( src );
  const SdGraphLinear *linear = dynamic_cast<const SdGraphLinear*>(src);
  if( linear ) mProp = linear->mProp;
  }



void SdGraphLinear::writeObject(QJsonObject &obj) const
  {
  SdGraph::writeObject( obj );
  mProp.write( obj );
  }



void SdGraphLinear::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mProp.read( obj );
  }



//Properties service [Изменение свойствами]
//Set properties of this object from prop
void SdGraphLinear::setProp(SdPropSelected &prop)
  {
  mProp = prop.mLineProp;
  }


//Get (append) properties from this object to prop
void SdGraphLinear::getProp(SdPropSelected &prop)
  {
  prop.mLineProp.append( mProp );
  prop.mFilledPropMask |= spsLineProp;
  }


bool SdGraphLinear::isVisible()
  {
  return mProp.mLayer.isVisible();
  }




void SdGraphLinear::setLayerUsage()
  {
  mProp.mLayer.setLayerUsage();
  }

