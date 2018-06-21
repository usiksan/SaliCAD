/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical traced object. Traced object is object which represents printed road or metallic hole.
*/
#include "SdGraphTraced.h"
#include "SdPItemPlate.h"

SdGraphTraced::SdGraphTraced()
  {

  }




SdPItemPlate *SdGraphTraced::getPlate() const
  {
  return dynamic_cast<SdPItemPlate*>(getParent());
  }




//Set flag to update rat net. Rat net will be clear and reconstructed
void SdGraphTraced::setDirtyRatNet()
  {
  getPlate()->setDirtyRatNet();
  }

bool SdGraphTraced::isMatchNetAndStratum(const QString netName, SdStratum stratum) const
  {
  Q_UNUSED(netName)
  Q_UNUSED(stratum)
  return false;
  }






void SdGraphTraced::draw(SdContext *dc)
  {
  drawStratum( dc, stmThrow );
  }
