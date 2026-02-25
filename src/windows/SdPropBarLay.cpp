/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base tool bar for property bars
*/
#include "SdPropBarLay.h"
#include "SdWCommand.h"
#include "objects/SdEnvir.h"
#include "objects/SdPulsar.h"

#include <QStyleFactory>
#include <QTimer>

SdPropBarLay::SdPropBarLay( const QString title ) :
  SdPropBar( title ),
  mEditObjectClass(dctCommon),
  mStratum(stmThrough)
  {
  insertAction( nullptr, SdWCommand::cmViewLayers );
  mLayer = new QComboBox();
  mLayer->setStyle( QStyleFactory::create("windows") );
  //mLayer->setEditable( true );
  mLayer->setMaxVisibleItems( 10 );
  addWidget( mLayer );
  mLayer->setMinimumWidth( 150 );
  mLayer->setToolTip( tr("Current layer") );

  //current layer selection changed. Send signal "prop changed"
  connect( mLayer, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, [this](int index) {
    Q_UNUSED(index)
    emit propChanged();
    });
  //When layer dialog completed
  connect( SdPulsar::sdPulsar, &SdPulsar::viewedLayers, this, &SdPropBarLay::updateViewedLayers );

  //fill new layers list
  QTimer::singleShot( 20, this, [this] () { updateViewedLayers(nullptr); } );
  }




void SdPropBarLay::setSelectedLayer(SdLayer *layer)
  {
  if( layer != nullptr ) {
    QString id = layer->id();
    int index = mLayer->findData( QVariant(id) );
    if( index >= 0 )
      mLayer->setCurrentIndex( index );
    else {
      //Switch on requred layer
      layer->stateSet( layerStateEdit );
      SdPulsar::sdPulsar->emitViewedLayers( layer );
      // updateViewedLayers( layer );
      // index = mLayer->findData( QVariant(id) );
      // if( index >= 0 )
      //   mLayer->setCurrentIndex( index );
      // else
      //   qDebug() << "setSelectedLayer failure";
      }
    }
  else {
    int index = mLayer->findText( QString() );
    if( index >= 0 )
      mLayer->setCurrentIndex( index );
    else
      mLayer->setCurrentIndex( -1 );
    }
  }




SdLayer *SdPropBarLay::getSelectedLayer()
  {
  int index = mLayer->currentIndex();
  if( index < 0 )
    return nullptr;
  QString id = mLayer->currentData().toString();
  if( id.isEmpty() )
    return nullptr;
  return SdEnvir::instance()->layerGet( id );
  }




//Visibility state of layers are changed. We need update list and preserve current layer selection
void SdPropBarLay::updateViewedLayers(SdLayer *currentLayer)
  {
  SdLayer *prevLayer = getSelectedLayer();
  //get current selection
  if( currentLayer == nullptr )
    currentLayer = prevLayer;
  refillLayers();
  setSelectedLayer( currentLayer );

  if( prevLayer != currentLayer )
    emit propChanged();
  }





void SdPropBarLay::setEditObject(SdProjectItem *pitem)
  {
  if( pitem == nullptr )
    setEditObject( dctCommon, stmThrough );
  else
    setEditObject( pitem->getClass(), pitem->getStratum() );
  }




void SdPropBarLay::setEditObject(SdClass theClass, SdPvStratum stratum)
  {
  if( mEditObjectClass != theClass || mStratum != stratum )  {
    mEditObjectClass = theClass;
    mStratum         = stratum;
    refillLayers();
    }
  }



void SdPropBarLay::refillLayers()
  {
  mLayer->clear();
  //fill new layers list
  SdEnvir::instance()->layerForEachConst( mEditObjectClass, [this] ( SdLayer *p ) -> bool {
    if( p->isEdited() && p->stratum().isIntersect(mStratum) ) {
      mLayer->addItem( p->name(), QVariant( p->id() ) );
      }
    return true;
    } );
  }



