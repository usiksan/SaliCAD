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
#include "SdPropBar.h"
#include "SdWCommand.h"
#include "objects/SdEnvir.h"
#include "objects/SdPulsar.h"

#include <QStyleFactory>
#include <QTimer>

SdPropBar::SdPropBar( const QString title ) :
  QToolBar( title )
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
  connect( SdPulsar::sdPulsar, &SdPulsar::viewedLayers, this, &SdPropBar::updateViewedLayers );

  //fill new layers list
  QTimer::singleShot( 20, this, [this] () { updateViewedLayers(nullptr); } );
  }




void SdPropBar::setSelectedLayer(SdLayer *layer)
  {
  if( layer != nullptr ) {
    QString id = layer->id();
    int index = mLayer->findData( QVariant(id) );
    if( index >= 0 )
      mLayer->setCurrentIndex( index );
    else {
      //Switch on requred layer
      layer->setState( layerStateEdit );
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




SdLayer *SdPropBar::getSelectedLayer()
  {
  int index = mLayer->currentIndex();
  if( index < 0 )
    return nullptr;
  QString id = mLayer->currentData().toString();
  if( id.isEmpty() )
    return nullptr;
  return sdEnvir->getLayer( id );
  }




//Visibility state of layers are changed. We need update list and preserve current layer selection
void SdPropBar::updateViewedLayers(SdLayer *currentLayer)
  {
  bool changeCurrentLayer = currentLayer != nullptr;
  //get current selection
  if( !changeCurrentLayer )
    currentLayer = getSelectedLayer();
  mLayer->clear();
  //fill new layers list
  for( SdLayer *p : std::as_const( sdEnvir->mLayerTable ) ) {
    if( p->isEdited() ) {
      mLayer->addItem( p->name(), QVariant( p->id() ) );
      }
    }
  setSelectedLayer( currentLayer );

  if( changeCurrentLayer )
    emit propChanged();
  }


