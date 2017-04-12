/*
  Проект "SaliBrick"
    Визуальное программирование микроконтроллеров
  Автор
    Сибилев А.С.
  Описание
*/
#include "SdPropBar.h"
#include "objects/SdEnvir.h"


SdPropBar::SdPropBar( const QString title ) :
  QToolBar( title )
  {
  mLayer = new QComboBox();
  mLayer->setEditable(false);
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
    else {
      mLayer->addItem( QString(), QVariant(QString()) );
      setSelectedLayer( layer );
      }
    }
  }




SdLayer *SdPropBar::getSelectedLayer()
  {
  QString id = mLayer->currentData().toString();
  if( id.isEmpty() )
    return 0;
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


