/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Layer property
*/
#include "SdPropLayer.h"
#include "SdEnvir.h"
#include "SdJsonIO.h"



//!
//! \brief SdPropLayer Constructor with layer identifier
//! \param id Layer identifier string to find and set the layer
//!
SdPropLayer::SdPropLayer(QString id) :
  mLayer( SdEnvir::instance()->layerGet(id) )
  {

  }

SdPropLayer::SdPropLayer(SdLayer *layer) :
  mLayer(layer)
  {
  if( mLayer == nullptr )
    mLayer = SdEnvir::instance()->layerGet(LID0_INVISIBLE);
  }




//!
//! \brief isVisible Check if layer is visible
//! \param otherSide If true, check paired layer for flipped components
//! \return True if layer (or its pair) is visible
//!
bool SdPropLayer::isVisible(bool otherSide) const
  {
  SdLayer *lay = layer( otherSide );
  if( lay )
    return lay->isVisible();
  return false;
  }




//!
//! \brief isEdited Check if layer is in edit mode
//! \return True if layer is currently editable
//!
bool SdPropLayer::isEdited() const
  {
  SdLayer *lay = layer( false );
  if( lay )
    return lay->isEdited();
  return false;
  }



//!
//! \brief set Set layer by identifier
//! \param id Layer identifier string to find and set
//!
void SdPropLayer::set(const QString id)
  {
  mLayer = SdEnvir::instance()->layerGet( id.isEmpty() ? LID0_INVISIBLE : id );
  }





//!
//! \brief layer Get layer pointer
//! \param otherSide If true, return paired layer for flipped components
//! \return Pointer to SdLayer object
//!
SdLayer *SdPropLayer::layer(bool otherSide) const
  {
  if( otherSide ) return mLayer->pair();
  return mLayer;
  }







//!
//! \brief setLayerUsage Mark layer as used (sets usage flag)
//!
void SdPropLayer::setLayerUsage() const
  {
  mLayer->setUsage();
  mLayer->pair()->setUsage();
  }






//!
//! \brief json Read layer property from JSON reader
//! \param name JSON key name
//! \param js   JSON reader object
//!
void SdPropLayer::json(const QString name, const SdJsonReader &js)
  {
  mLayer = SdEnvir::instance()->layerGet( js.object().value(name).toString() );
  }



