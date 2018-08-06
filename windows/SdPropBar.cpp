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

  //fill new layers list
  for( SdLayer *p : sdEnvir->mLayerTable ) {
    if( p->isEdited() ) {
      mLayer->addItem( p->name(), QVariant( p->id() ) );
      }
    }

  //current layer selection changed. Send signal "prop changed"
  connect( mLayer, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int index){
    Q_UNUSED(index)
    emit propChanged();
    });
  //When layer dialog completed
  connect( SdPulsar::sdPulsar, &SdPulsar::viewedLayers, this, &SdPropBar::updateViewedLayers );
  }




void SdPropBar::setSelectedLayer(SdLayer *layer)
  {
  if( layer ) {
    QString id = layer->id();
    int index = mLayer->findData( QVariant(id) );
    if( index >= 0 )
      mLayer->setCurrentIndex( index );
    }
  else {
    int index = mLayer->findText( QString() );
    if( index >= 0 )
      mLayer->setCurrentIndex( index );
    else
      mLayer->setCurrentIndex( -1 );
//      {
//      mLayer->addItem( QString(), QVariant(QString()) );
//      setSelectedLayer( layer );
//      }
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
void SdPropBar::updateViewedLayers()
  {
  //get current selection
  SdLayer *layer = getSelectedLayer();
  mLayer->clear();
  //fill new layers list
  for( SdLayer *p : sdEnvir->mLayerTable ) {
    if( p->isEdited() ) {
      mLayer->addItem( p->name(), QVariant( p->id() ) );
      }
    }
  setSelectedLayer( layer );
  }


