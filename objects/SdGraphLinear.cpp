/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdGraphLinear.h"

SdGraphLinear::SdGraphLinear()
  {

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



void SdGraphLinear::setProp(SdProp &prop)
  {
  mProp = prop.mLineProp;
  }



void SdGraphLinear::getProp(SdProp &prop)
  {
  prop.mLineProp.append( mProp );
  }


bool SdGraphLinear::isAble()
  {
  return mProp.mLayer.layer(false)->isEdited();
  }

