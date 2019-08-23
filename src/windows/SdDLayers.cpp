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
#include "SdDLayerCreate.h"
#include "SdDHelp.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDir>


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
  sdEnvir->resetLayerUsage();

  //Accum usages
  if( mProject != nullptr )
    mProject->accumLayerUsage();


  ui->mLayerList->setColumnCount(6);
  ui->mLayerList->setColumnWidth( 0, 200 );
  ui->mLayerList->setColumnWidth( 1, 70 );
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

  //By default stratum count disabled. When set stratum count it is enabled
  ui->mStratumCount->setEnabled(false);

  fillLayerList();
  }



SdDLayers::~SdDLayers()
  {
  delete ui;
  }



//Stratum count. By default stratum count disabled. When set stratum count it is enabled
void SdDLayers::setStratumCount(int c)
  {
  ui->mStratumCount->setEnabled(true);
  ui->mStratumCount->setText( QString::number(c) );
  }




int SdDLayers::getStratumCount() const
  {
  return SdUtil::iLimit( ui->mStratumCount->text().toInt(), 1, 30 );
  }





//Load layer list (enable-disable status)
void SdDLayers::loadLayerList(const QString fname)
  {
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
      sdEnvir->getLayer(id)->setState( static_cast<SdLayerState>(state) );
      }
    }
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
  ui->mLayerList->setRowHeight( row, 20 );

  //Layer name
  QTableWidgetItem *item = new QTableWidgetItem( layer->name() );
  ui->mLayerList->setItem( row, 0, item );

  //Layer trace
  item = new QTableWidgetItem( layerTrace( layer ) );
  item->setFlags( Qt::ItemIsEnabled );
  ui->mLayerList->setItem( row, 1, item );

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
  item->setBackground( layer->color() );
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
  SdDHelp::help( QString("SdDLayers.htm"), this );
  }




//Create new layer
void SdDLayers::cmCreate()
  {
  SdDLayerCreate cr(this);
  if( cr.exec() ) {
    //New layer created, append it to internal list
    mList.append( cr.layerId() );
    //New layer created, append it to visual list
    appendLyaerToVisualList( cr.layerId() );
    //Mark as usage for visibility
    sdEnvir->getLayer( cr.layerId() )->setUsage();
    //Setup stratum
    sdEnvir->getLayer( cr.layerId() )->setStratum( cr.layerStratum() );
    }
  }




//Delete selected layer
void SdDLayers::cmDelete()
  {
  //Current layer
  int index = ui->mLayerList->currentRow();
  if( index >= 0 && index < mList.count() ) {
    SdLayer *layer = sdEnvir->getLayer( mList.at(index) );
    //Show acknowledge to delete layer
    if( QMessageBox::question( this, tr("Warning!"), tr("Are You sure to delete layer \"%1\"").arg(layer->name()), QMessageBox::Yes | QMessageBox::No  ) == QMessageBox::Yes ) {
      //Test layer to able be deleted
      sdEnvir->resetLayerUsage();
      for( SdProjectPtr prj : sdProjectList )
        prj->accumLayerUsage();
      if( layer->isUsage() || layer->pair() != layer )
        QMessageBox::warning( this, tr("Error"), tr("Layer \"%1\" is used in some projects. It can't be deleted.").arg(layer->name()) );
      else {
        ui->mLayerList->removeRow( index );
        mList.removeAt( index );
        sdEnvir->mLayerTable.remove( layer->id() );
        sdEnvir->resetForCache();
        delete layer;
        }
      }
    }
  else
    QMessageBox::warning( this, tr("Error"), tr("Select layer to delete") );
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
                                                sdEnvir->mPatternPath,
                                                QString("Layers list (*%1)").arg(SD_LAYER_LIST_EXTENSION) );
  if( !fname.isEmpty() ) {
    if( !fname.endsWith( QStringLiteral(SD_LAYER_LIST_EXTENSION)) )
      fname.append( QStringLiteral(SD_LAYER_LIST_EXTENSION) );

    //Load layer list from file
    loadLayerList( fname );

    //Update visula list
    fillLayerList();
    }
  }




//Save list of edit and visible layers
void SdDLayers::cmSave()
  {
  //Create pattern dir for layers lists
  QDir dir;
  dir.mkpath( sdEnvir->mPatternPath );

  //Select file name to save list
  QString fname = QFileDialog::getSaveFileName( this, tr("Enter file name to save layer state list"),
                                                sdEnvir->mPatternPath,
                                                QString("Layers list (*%1)").arg(SD_LAYER_LIST_EXTENSION) );
  if( !fname.isEmpty() ) {
    if( !fname.endsWith( QStringLiteral(SD_LAYER_LIST_EXTENSION)) )
      fname.append( QStringLiteral(SD_LAYER_LIST_EXTENSION) );
    //Make visible layer list
    QJsonArray ar;
    for( const QString &id : mList ) {
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
  if( column == 1 ) {
    //Layer trace
    SdLayerTrace trace = sdEnvir->getLayer(id)->trace();
    switch( trace ) {
      //Layer not tracing [Слой не трассировочный]
      case layerTraceNone : trace = layerTracePad; break;
      //Layer for pads
      case layerTracePad :  trace = layerTraceMask; break;
      //Layer for mask
      case layerTraceMask : trace = layerTraceStencil; break;
      //Layer for stensil apertures
      case layerTraceStencil : trace = layerTraceHole; break;
      //Layer for holes
      case layerTraceHole : trace = layerTraceRoad; break;
      //Layer for wires
      case layerTraceRoad : trace = layerTracePolygon; break;
      //Layer for polygons
      case layerTracePolygon : trace = layerTraceBoundary; break;
      //Layer for tracing boundary
      case layerTraceBoundary : trace = layerTraceKeepout; break;
      //Layer for tracing keepout
      case layerTraceKeepout :
      case layerTraceLast : trace = layerTraceNone; break;
      }
    sdEnvir->getLayer(id)->setTrace( trace );
    //Change in table
    QTableWidgetItem *item = ui->mLayerList->item(row,column);
    if( item != nullptr )
      item->setText( layerTrace(sdEnvir->getLayer(id)) );
    }
  else if( column == 2 ) {
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
  else if( column == 3 ) {
    //Icon for visible state. Press on icon change visible state
    SdLayerState state = sdEnvir->getLayer(id)->state();
    if( state == layerStateOff ) state = layerStateOn;
    else state = layerStateOff;
    sdEnvir->getLayer(id)->setState( state );
    //Change in table
    QTableWidgetItem *item = ui->mLayerList->item(row,2);
    if( item != nullptr )
      item->setText( layerState(sdEnvir->getLayer(id)) );
    //Change visibility
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
        item->setBackground( color );
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





//Return textual representation of layer trace
QString SdDLayers::layerTrace(SdLayer *layer)
  {
  switch( layer->trace() ) {
    case layerTraceNone :     //Layer not tracing [Слой не трассировочный]
      return tr("---");
    case layerTracePad :      //Layer for pads
      return tr("pad");
    case layerTraceMask :     //Layer for mask
      return tr("mask");
    case layerTraceStencil :  //Layer for stensil apertures
      return tr("stensil");
    case layerTraceHole :     //Layer for holes
      return tr("hole");
    case layerTraceRoad :     //Layer for wires
      return tr("road");
    case layerTracePolygon :  //Layer for polygons
      return tr("polygon");
    case layerTraceBoundary : //Layer for tracing boundary
      return tr("bound");
    case layerTraceKeepout :  //Layer for tracing keepout
      return tr("keepout");
    case layerTraceLast :
      return tr("last");
    }
  return tr("---");
  }
