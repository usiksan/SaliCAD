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
#include "objects/SdPItemPlate.h"

#include <algorithm>

SdPropBarStratum::SdPropBarStratum(const QString title) :
  SdPropBar(title),
  mPlate(nullptr)
  {
  }




void SdPropBarStratum::setPlateAndTrace(SdPItemPlate *plate, SdLayerTrace trace)
  {
  if( mPlate != plate || mLayerTrace != trace ) {
    //Do only if changed, else do nothing
    mPlate        = plate;
    mLayerTrace   = trace;
    updateViewedLayers();
    }
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
  return layer->stratum();
//  if( layer->stratum() == 0 )
//    return 0;
//  int stratumIndex = SdStratum::stratumIndex( layer->stratum() );
//  return 1 << stratumIndex;
  }





void SdPropBarStratum::updateViewedLayers()
  {
  SdStratum curStratum = getSelectedStratum();
  //fill new layers list
  int c = mPlate == nullptr ? 2 : mPlate->stratumCount();
  int stratumMask = SdStratum::stratumStack( c );
  //Remove previous layers
  mLayer->clear();
  //Accumulate available layers to signalLayers list
  QList<SdLayer*> signalLayers;
  for( SdLayer *p : qAsConst( sdEnvir->mLayerTable ) ) {
    if( p->trace() == mLayerTrace && (p->stratum() & stratumMask) && p->isEdited() ) {
      signalLayers.append( p );
      //mLayer->addItem( p->name(), QVariant( p->id() ) );
      }
    }

  //Sort layer list in non-descending order of layers
  std::stable_sort( signalLayers.begin(), signalLayers.end(), [] ( SdLayer *p1, SdLayer *p2 ) -> bool { return p1->stratum() < p2->stratum(); } );

  //Fill visual list
  for( SdLayer *p : signalLayers )
    mLayer->addItem( p->name(), QVariant( p->id() ) );

  //Select current stratum layer
  setSelectedStratum(curStratum);
  }


