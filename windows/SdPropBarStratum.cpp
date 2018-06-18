/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdPropBarStratum.h"
#include "objects/SdUtil.h"
#include "objects/SdEnvir.h"

SdPropBarStratum::SdPropBarStratum(const QString title) :
  SdPropBar(title)
  {

  }




void SdPropBarStratum::setStratumCountAndTrace(int stratumCount, SdLayerTrace trace)
  {
  mStratumCount = stratumCount;
  mLayerTrace   = trace;
  updateViewedLayers();
  }





void SdPropBarStratum::setSelectedStratum(SdStratum stratum)
  {
  for( int i = 0; i < mLayer->count(); i++ ) {
    QString id = mLayer->itemData(i).toString();
    if( id.isEmpty() )
      continue;
    SdLayer *layer = sdEnvir->getLayer( id );
    if( stratum & layer->stratum() ) {
      mLayer->setCurrentIndex(i);
      return;
      }
    }
  mLayer->setCurrentText(QString());
  }





SdStratum SdPropBarStratum::getSelectedStratum()
  {
  int index = mLayer->currentIndex();
  if( index < 0 )
    return 0;
  QString id = mLayer->currentData().toString();
  if( id.isEmpty() )
    return 0;
  SdLayer *layer = sdEnvir->getLayer( id );
  if( layer->stratum() == 0 )
    return 0;
  int stratumIndex = SdStratum::stratumIndex( layer->stratum() );
  return 1 << stratumIndex;
  }





void SdPropBarStratum::updateViewedLayers()
  {
  SdStratum curStratum = getSelectedStratum();
  //fill new layers list
  int stratumMask = ~(-1 << mStratumCount);
  for( SdLayer *p : sdEnvir->mLayerTable ) {
    if( p->trace() == mLayerTrace && (p->stratum() & stratumMask) && p->isEdited() ) {
      mLayer->addItem( p->name(), QVariant( p->id() ) );
      }
    }
  setSelectedStratum(curStratum);
  }


