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
#include "SdJsonIO.h"

SdGraphLinear::SdGraphLinear() :
  SdGraph()
  {

  }

SdGraphLinear::SdGraphLinear(const SdPropLine &propLine) :
  SdGraph()
  {
  mProp = propLine;
  }




//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphLinear::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraph::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphLinear> linear(src);
  if( linear.isValid() )
    mProp = linear->mProp;
  }




void SdGraphLinear::json(SdJsonWriter &js) const
  {
  mProp.json( js );
  SdGraph::json( js );
  }



void SdGraphLinear::json(const SdJsonReader &js)
  {
  mProp.json( js );
  SdGraph::json( js );
  }








//Properties service [Изменение свойствами]
//Set properties of this object from prop
void SdGraphLinear::setProp(SdPropSelected &prop)
  {
  prop.mLineProp.store( mProp );
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

