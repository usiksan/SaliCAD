/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Layers maintence dialog
*/
#include "objects/SdLayer.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraph.h"
#include "SdDLayers.h"
#include "ui_SdDLayers.h"
#include "SdDLayerList.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


int SdDLayers::mShow; //Define show all, actual or used layers

SdDLayers::SdDLayers(SdProject *prj, QWidget *parent) :
  QDialog(parent),
  mProject(prj),
  ui(new Ui::SdDLayers)
  {
  ui->setupUi(this);

  ui->mShowActual->setChecked( mShow == 0 );
  ui->mShowUsed->setChecked( mShow == 1 );
  ui->mShowAll->setChecked( mShow == 2 );

  connect( ui->mClose,      &QPushButton::clicked, this, &SdDLayers::close );
  connect( ui->mCreate,     &QPushButton::clicked, this, &SdDLayers::cmCreate );
  connect( ui->mDelete,     &QPushButton::clicked, this, &SdDLayers::cmDelete );
  connect( ui->mHelp,       &QPushButton::clicked, this, &SdDLayers::cmHelp );
  connect( ui->mHideAll,    &QPushButton::clicked, this, &SdDLayers::cmHideAll );
  connect( ui->mSwitchAll,  &QPushButton::clicked, this, &SdDLayers::cmSwitchAll );
  connect( ui->mLoad,       &QPushButton::clicked, this, &SdDLayers::cmLoad );
  connect( ui->mSave,       &QPushButton::clicked, this, &SdDLayers::cmSave );
  connect( ui->mShowActual, &QPushButton::clicked, this, &SdDLayers::cmShowActual );
  connect( ui->mShowAll,    &QPushButton::clicked, this, &SdDLayers::cmShowAll );
  connect( ui->mShowUsed,   &QPushButton::clicked, this, &SdDLayers::cmShowUsed );

  connect( ui->mLayerList,  &QTableWidget::cellClicked, this, &SdDLayers::onCellClicked );
  connect( ui->mLayerList,  &QTableWidget::itemChanged, this, &SdDLayers::onItemChanged );

  //Fill actual layer list
  for( int i = 0; sdLayerDescrActual[i].mId != nullptr; i++ )
    mActualList.insert( QString(sdLayerDescrActual[i].mId) );

  //Accum usaged layers
  //At first - clear all usages
  for( SdLayer *layer : sdEnvir->mLayerTable )
    layer->resetUsage();

  //Accum usages
  mProject->forEach( dctAll, [] ( SdObject *obj ) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph != nullptr )
      graph->setLayerUsage();
    return true;
    } );


  ui->mLayerList->setColumnCount(6);
  ui->mLayerList->setColumnWidth( 0, 200 );
  ui->mLayerList->setColumnWidth( 1, 50 );
  ui->mLayerList->setColumnWidth( 2, 50 );
  ui->mLayerList->setColumnWidth( 3, 50 );
  ui->mLayerList->setColumnWidth( 4, 50 );
  ui->mLayerList->setColumnWidth( 5, 200 );

  //Set column header
  QStringList header;
  header << tr("Lyaer name") << tr("Trace") << tr("State") << tr("Visible") << tr("Color") << tr("Pair name");
  ui->mLayerList->setHorizontalHeaderLabels( header );

  //Hide row header
  ui->mLayerList->verticalHeader()->hide();

  fillLayerList();
  }



SdDLayers::~SdDLayers()
  {
  delete ui;
  }




//Construct visual layer
void SdDLayers::fillLayerList()
  {
  //Clear previous list
  mList.clear();

  if( mShow == 0 ) {
    //Actual list
    for( SdLayer *layer : sdEnvir->mLayerTable ) {
      if( layer->isUsage() || mActualList.contains(layer->id()) )
        mList.append( layer->id() );
      }
    }
  else if( mShow == 1 ) {
    //Used list
    for( SdLayer *layer : sdEnvir->mLayerTable ) {
      if( layer->isUsage() )
        mList.append( layer->id() );
      }
    }
  else {
    //All list
    for( SdLayer *layer : sdEnvir->mLayerTable )
      mList.append( layer->id() );
    }

  ui->mLayerList->setRowCount(0);
  for( QString id : mList )
    appendLyaerToVisualList( id );
  }





//Internal function
//append layer with desired Id to visual list (i.e. table)
void SdDLayers::appendLyaerToVisualList(const QString id)
  {
  SdLayer *layer = sdEnvir->getLayer( id );
  //Append row to table
  int row = ui->mLayerList->rowCount();
  ui->mLayerList->insertRow( row );
  //Layer name
  QTableWidgetItem *item = new QTableWidgetItem( layer->name() );
  ui->mLayerList->setItem( row, 0, item );
  //Layer trace
  //QString trace;
  //Layer edit
  item = new QTableWidgetItem( layerState(layer) );
  item->setFlags( Qt::ItemIsEnabled );
  ui->mLayerList->setItem( row, 2, item );
  //Layer visible
  item = new QTableWidgetItem();
  item->setFlags( Qt::ItemIsEnabled );
  item->setIcon( QIcon( layer->isVisible() ? QString(":/pic/lampEnable.png") : QString(":/pic/lampDisable.png") ) );
  ui->mLayerList->setItem( row, 3, item );
  //Layer color
  item = new QTableWidgetItem( "      " );
  item->setFlags( Qt::ItemIsEnabled );
  item->setBackgroundColor( layer->color() );
  ui->mLayerList->setItem( row, 4, item );
  //Layer pair
  if( layer != layer->pair() )
    item = new QTableWidgetItem( layer->pair()->name() );
  else
    item = new QTableWidgetItem();
  item->setFlags( Qt::ItemIsEnabled );
  ui->mLayerList->setItem( row, 5, item );
  }





//Show help
void SdDLayers::cmHelp()
  {

  }




//Create new layer
void SdDLayers::cmCreate()
  {

  }




//Delete selected layer
void SdDLayers::cmDelete()
  {
  //Current layer
  int index = ui->mLayerList->currentRow();
  if( index >= 0 && index < mList.count() ) {

    }
  else

  //Show acknowledge to delete layer

  }





//Show all known layers
void SdDLayers::cmShowAll()
  {
  mShow = 2;
  fillLayerList();
  }




//Show only actual layers
//it is used layers and common used layers
void SdDLayers::cmShowActual()
  {
  mShow = 0;
  fillLayerList();
  }





//Show only used layers
void SdDLayers::cmShowUsed()
  {
  mShow = 1;
  fillLayerList();
  }







//Set state of all layers to Hide
void SdDLayers::cmHideAll()
  {
  for( QString id : mList )
    sdEnvir->getLayer(id)->setState( layerStateOff );
  fillLayerList();
  }






//Switch state of all layears to next state
void SdDLayers::cmSwitchAll()
  {
  for( QString id : mList ) {
    SdLayerState state = sdEnvir->getLayer(id)->state();
    if( state == layerStateOff ) state = layerStateOn;
    else if( state == layerStateOn ) state = layerStateEdit;
    else state = layerStateOff;
    sdEnvir->getLayer(id)->setState( state );
    }
  fillLayerList();
  }



//Load list of layer states
void SdDLayers::cmLoad()
  {
  QString fname = QFileDialog::getOpenFileName( this, tr("Select file name to load layer state list"),
                                                QString(),
                                                QString("Layers list (*%1)").arg(SD_LAYER_LIST_EXTENSION) );
  if( !fname.isEmpty() ) {
    if( !fname.endsWith( QStringLiteral(SD_LAYER_LIST_EXTENSION)) )
      fname.append( QStringLiteral(SD_LAYER_LIST_EXTENSION) );

    //Load layer state list
    QFile file( fname );
    if( file.open(QIODevice::ReadOnly) ) {
      QJsonDocument doc = QJsonDocument::fromJson( file.readAll() );
      QJsonArray ar = doc.array();

      //Switch off all layers
      for( SdLayerPtr ptr : sdEnvir->mLayerTable )
        ptr->setState( layerStateOff );

      //Setup layers state
      for( int i = 0; i < ar.count(); i++ ) {
        QJsonObject obj = ar.at(i).toObject();
        QString id = obj.value( QStringLiteral("id") ).toString();
        int state = obj.value( QStringLiteral("state") ).toInt();
        sdEnvir->getLayer(id)->setState( (SdLayerState)state );
        }

      //Update visula list
      fillLayerList();
      }
    }
  }




//Save list of edit and visible layers
void SdDLayers::cmSave()
  {
  //Select file name to save list
  QString fname = QFileDialog::getSaveFileName( this, tr("Enter file name to save layer state list"),
                                                QString(),
                                                QString("Layers list (*%1)").arg(SD_LAYER_LIST_EXTENSION) );
  if( !fname.isEmpty() ) {
    if( !fname.endsWith( QStringLiteral(SD_LAYER_LIST_EXTENSION)) )
      fname.append( QStringLiteral(SD_LAYER_LIST_EXTENSION) );
    //Make visible layer list
    QJsonArray ar;
    for( const QString id : mList ) {
      int state = sdEnvir->getLayer(id)->state();
      QJsonObject obj;
      obj.insert( QStringLiteral("id"), id );
      obj.insert( QStringLiteral("state"), state );
      ar.append( obj );
      }

    //Layer list completed
    QFile file( fname );
    if( file.open( QIODevice::WriteOnly ) ) {
      QJsonDocument doc( ar );
      file.write( doc.toJson() );
      }
    }
  }






void SdDLayers::onCellClicked(int row, int column)
  {
  QString id = mList.at(row);
  if( column == 2 ) {
    //State field
    SdLayerState state = sdEnvir->getLayer(id)->state();
    if( state == layerStateOff ) state = layerStateOn;
    else if( state == layerStateOn ) state = layerStateEdit;
    else state = layerStateOff;
    sdEnvir->getLayer(id)->setState( state );
    //Change in table
    QTableWidgetItem *item = ui->mLayerList->item(row,column);
    if( item != nullptr )
      item->setText( layerState(sdEnvir->getLayer(id)) );
    item = ui->mLayerList->item( row, 3 );
    if( item != nullptr )
      item->setIcon( QIcon( sdEnvir->getLayer(id)->isVisible() ? QString(":/pic/lampEnable.png") : QString(":/pic/lampDisable.png") ) );
    }
  else if( column == 4 ) {
    //Layer color
    QColor color = QColorDialog::getColor( sdEnvir->getLayer(id)->color(), this, tr("Select layer color") );
    if( color.isValid() ) {
      sdEnvir->getLayer(id)->setColor( color.rgba() );
      //Change in table
      QTableWidgetItem *item = ui->mLayerList->item(row,column);
      if( item != nullptr ) {
        item->setBackgroundColor( color );
        }
      }
    }
  else if( column == 5 ) {
    //Layer pair
    QString pair = sdEnvir->getLayer(id)->pair()->id();
    //If pair not assigned then clear pair id
    if( pair == id )
      pair.clear();
    SdDLayerList pairList( pair, this );
    int r = pairList.exec();
    if( r == SdDLayerList_remove )
      //Remove pair
      sdEnvir->getLayer(id)->setPair( nullptr );
    else if( r == SdDLayerList_assign ) {
      //Get selected pair
      pair = pairList.pair();
      if( !pair.isEmpty() )
        sdEnvir->getLayer(id)->setPair( sdEnvir->getLayer(pair) );
      }
    //Replace pair name in visual representation
    QTableWidgetItem *item = ui->mLayerList->item(row,column);
    if( item != nullptr )
      item->setText( sdEnvir->getLayer(id)->pair()->name() );
    }
  //
  }




void SdDLayers::onItemChanged(QTableWidgetItem *item)
  {
  if( item != nullptr ) {
    int row = ui->mLayerList->row( item );
    int column = ui->mLayerList->column( item );
    if( column == 0 ) {
      //Change layer visual name
      //Visual name nor on that not affects, its only visual presentation
      QString id = mList.at(row);
      sdEnvir->getLayer(id)->setName( item->text() );
      }
    }
  }



//Return textual representation of layer state
QString SdDLayers::layerState(SdLayer *layer)
  {
  if( layer->state() == layerStateOff ) return tr("Off");
  if( layer->state() == layerStateOn ) return tr("On");
  return tr("Edit");
  }
