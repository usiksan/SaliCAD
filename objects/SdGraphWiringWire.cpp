/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdGraphWiringWire.h"
#include "SdContainerSheetNet.h"

SdGraphWiringWire::SdGraphWiringWire()
  {

  }

SdContainerSheetNet *SdGraphWiringWire::getNet() const
  {
  return dynamic_cast<SdContainerSheetNet*>( getParent() );
  }



//Get wire name
QString SdGraphWiringWire::getWireName() const
  {
  SdContainerSheetNet *net = getNet();
  if( net ) return net->getNetName();
  return QString();
  }
