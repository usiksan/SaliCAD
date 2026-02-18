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





//Accumulate traced linked to point
void SdGraphTraced::accumLinkedTrace(SdGraphTraced *from, SdPoint a, QString netName, SdSelector *sel)
  {
  getPlate()->forEach( dctTraced, [from,a,netName,sel] ( SdObject *obj ) -> bool {
    if( obj != from ) {
      SdPtr<SdGraphTraced> traced( obj );
      if( traced.isValid() && traced->isLinked( a, from->stratum(), netName ) )
        sel->insert( traced.ptr() );
      }
    return true;
    } );
  }




bool SdGraphTraced::isMatchNetAndStratum(const QString netName, SdPvStratum stratum) const
  {
  Q_UNUSED(netName)
  Q_UNUSED(stratum)
  return false;
  }






void SdGraphTraced::draw(SdContext *dc)
  {
  drawStratum( dc, stmThrough );
  }
