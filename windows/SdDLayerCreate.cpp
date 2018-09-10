/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdDLayerCreate.h"
#include "ui_SdDLayerCreate.h"
#include "objects/SdLayer.h"
#include "objects/SdEnvir.h"

SdDLayerCreate::SdDLayerCreate(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDLayerCreate)
  {
  ui->setupUi(this);

  //Fill class list
  for( int i = 0; sdLayerLevel0[i].mLid != nullptr; i++ )
    ui->mClassList->addItem( tr(sdLayerLevel0[i].mTranslate) );
  ui->mClassList->addItem( tr("User id") );

  //Fill modifier list
  for( int i = 0; sdLayerLevel1[i].mLid != nullptr; i++ )
    ui->mModifierList->addItem( tr(sdLayerLevel1[i].mTranslate) );

  //By default id editor line disabled
  ui->mLayerId->setReadOnly(true);

  connect( ui->mClassList, &QListWidget::currentRowChanged, this, &SdDLayerCreate::onLayerIdConstructed );
  connect( ui->mModifierList, &QListWidget::currentRowChanged, this, &SdDLayerCreate::onLayerIdConstructed );
  }





SdDLayerCreate::~SdDLayerCreate()
  {
  delete ui;
  }




QString SdDLayerCreate::layerId() const
  {
  return ui->mLayerId->text();
  }




int SdDLayerCreate::layerStratum() const
  {
  int rowModifier = ui->mModifierList->currentRow();
  if( rowModifier > 0 )
    return sdLayerLevel1[rowModifier].mStratum;
  return stmThrough;
  }





//On layer constructed
void SdDLayerCreate::onLayerIdConstructed(int)
  {
  //Get selction on both lists
  int rowClass = ui->mClassList->currentRow();
  int rowModifier = ui->mModifierList->currentRow();

  //Build layer id
  if( rowClass < 0 ) {
    //No layer id
    ui->mLayerId->clear();
    ui->mLayerName->clear();
    ui->mLayerId->setReadOnly(true);
    }
  else if( sdLayerLevel0[rowClass].mLid == nullptr ) {
    //User id
    ui->mLayerId->clear();
    ui->mLayerName->clear();
    ui->mLayerId->setReadOnly(false);
    }
  else {
    ui->mLayerId->setReadOnly(true);
    QString lid0( sdLayerLevel0[rowClass].mLid );
    if( rowModifier > 0 ) {
      QString lid1( sdLayerLevel1[rowModifier].mLid );
      ui->mLayerId->setText( lid0 + lid1 );
      ui->mLayerName->setText( sdEnvir->layerId2NameLevel0(lid0) + QString(" ") + sdEnvir->layerId2NameLevel1(lid1) );
      }
    else {
      //No modifier
      ui->mLayerId->setText( lid0 );
      ui->mLayerName->setText( sdEnvir->layerId2NameLevel0(lid0) );
      }
    }
  }











//Create layer
void SdDLayerCreate::accept()
  {
  QString id = ui->mLayerId->text();
  if( !id.isEmpty() ) {
    sdEnvir->getLayer( id );
    done(1);
    }
  else done(0);
  }
