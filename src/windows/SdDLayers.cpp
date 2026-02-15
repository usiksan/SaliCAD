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
#include <QSlider>


int SdDLayers::mShow; //Define show all, actual or used layers

SdDLayers::SdDLayers(SdClass editObjectClass, SdProject *prj, QWidget *parent) :
  QDialog(parent),
  mEditObjectClass(editObjectClass),
  mProject(prj),
  mStratumMask(stmTop|stmBottom),
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

  connect( ui->mStratumCount, &QSlider::valueChanged, this, &SdDLayers::onStratumCountChange );

  //Fill actual layer list
  // SdEnvir::instance()->layerForEachConst( mEditObjectClass, [this] (SdLayer *layer) -> bool {
  //   mActualList.insert( layer->id() );
  //   return true;
  //   });
  for( int i = 0; sdLayerDescrDefault[i].mId != nullptr; i++ )
    mActualList.insert( QString(sdLayerDescrDefault[i].mId) );

  //Accum usaged layers
  //At first - clear all usages
  SdEnvir::instance()->resetLayerUsage();

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
  c = qBound( 2, c, 30 );
  ui->mStratumCount->setEnabled(true);
  ui->mStratumCount->setValue( c / 2 );
  onStratumCountChange( c / 2 );
  }




int SdDLayers::getStratumCount() const
  {
  return qBound( 2, ui->mStratumCount->value() * 2, 30 );
  }



//!
//! \brief currentLayer Returns current selected layer if it is has edited state
//! \return             Current selected layer if it is has edited state
//!
SdLayer *SdDLayers::currentLayer() const
  {
  int index = ui->mLayerList->currentRow();
  if( index >= 0 && index < mList.count() ) {
    SdLayer *layer = SdEnvir::instance()->layerGet( mList.at(index) );
    if( layer && layer->isEdited() )
      return layer;
    }
  return nullptr;
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
    SdEnvir::instance()->layerForEach( dctCommon, [] (SdLayer *layer) ->bool {
      layer->stateSet( layerStateOff );
      return true;
      });

    //Setup layers state
    for( int i = 0; i < ar.count(); i++ ) {
      QJsonObject obj = ar.at(i).toObject();
      QString id = obj.value( QStringLiteral("id") ).toString();
      int state = obj.value( QStringLiteral("state") ).toInt();
      SdEnvir::instance()->layerGet(id)->stateSet( static_cast<SdLayerState>(state) );
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
    SdEnvir::instance()->layerForEachConst( mEditObjectClass, [this] (SdLayer *layer) ->bool {
      if( (layer->isUsage() || mActualList.contains(layer->id()))  && (layer->stratum() & mStratumMask) )
        mList.append( layer->id() );
      return true;
      });
    }
  else if( mShow == 1 ) {
    //Used list
    SdEnvir::instance()->layerForEachConst( mEditObjectClass, [this] (SdLayer *layer) ->bool {
      if( layer->isUsage()  && (layer->stratum() & mStratumMask) )
        mList.append( layer->id() );
      return true;
      });
    }
  else {
    //All list
    SdEnvir::instance()->layerForEachConst( mEditObjectClass, [this] (SdLayer *layer) ->bool {
      if( (layer->stratum() & mStratumMask) )
        mList.append( layer->id() );
      return true;
      });
    }

  //Sorting layers by index
  std::sort( mList.begin(), mList.end(), [] ( const QString &id1, const QString &id2 ) -> bool {
    SdLayer *layer1 = SdEnvir::instance()->layerGet( id1 );
    SdLayer *layer2 = SdEnvir::instance()->layerGet( id2 );
    return layer1->index() < layer2->index();
    });

  ui->mLayerList->setRowCount(0);
  for( auto const &id : std::as_const( mList ) )
    appendLyaerToVisualList( id );
  }





//Internal function
//append layer with desired Id to visual list (i.e. table)
void SdDLayers::appendLyaerToVisualList(const QString id)
  {
  SdLayer *layer = SdEnvir::instance()->layerGet( id );

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
    //Mark as usage for visibility
    SdEnvir::instance()->layerGet( cr.layerId() )->setUsage();
    //New layer created, append it to internal list
    mList.append( cr.layerId() );
    //New layer created, append it to visual list
    appendLyaerToVisualList( cr.layerId() );
    }
  }




//Delete selected layer
void SdDLayers::cmDelete()
  {
  //Current layer
  int index = ui->mLayerList->currentRow();
  if( index >= 0 && index < mList.count() ) {
    SdLayer *layer = SdEnvir::instance()->layerGet( mList.at(index) );
    //Show acknowledge to delete layer
    if( QMessageBox::question( this, tr("Warning!"), tr("Are You sure to delete layer \"%1\"").arg(layer->name()), QMessageBox::Yes | QMessageBox::No  ) == QMessageBox::Yes ) {
      //Test layer to able be deleted
      SdEnvir::instance()->resetLayerUsage();
      for( SdProjectPtr prj : sdProjectList )
        prj->accumLayerUsage();
      if( layer->isUsage() || layer->pair() != layer )
        QMessageBox::warning( this, tr("Error"), tr("Layer \"%1\" is used in some projects. It can't be deleted.").arg(layer->name()) );
      else {
        ui->mLayerList->removeRow( index );
        mList.removeAt( index );
        SdEnvir::instance()->layerRemove( layer->id() );
        SdEnvir::instance()->resetForCache();
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
  for( const QString &id : std::as_const(mList) )
    SdEnvir::instance()->layerGet(id)->stateSet( layerStateOff );
  fillLayerList();
  }






//Switch state of all layears to next state
void SdDLayers::cmSwitchAll()
  {
  for( const QString &id : std::as_const(mList) ) {
    SdLayerState state = SdEnvir::instance()->layerGet(id)->state();
    if( state == layerStateOff ) state = layerStateOn;
    else if( state == layerStateOn ) state = layerStateEdit;
    else state = layerStateOff;
    SdEnvir::instance()->layerGet(id)->stateSet( state );
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

    //Load layer list from file
    loadLayerList( fname );

    //Update visula list
    fillLayerList();
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
    for( const QString &id : mList ) {
      int state = SdEnvir::instance()->layerGet(id)->state();
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
#if 0  //Disable trace changing
    SdLayerTrace trace = SdEnvir::instance()->layerGet(id)->trace();
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
    SdEnvir::instance()->layerGet(id)->setTrace( trace );
    //Change in table
    QTableWidgetItem *item = ui->mLayerList->item(row,column);
    if( item != nullptr )
      item->setText( layerTrace(SdEnvir::instance()->layerGet(id)) );
#endif
    }
  else if( column == 2 ) {
    //State field
    SdLayerState state = SdEnvir::instance()->layerGet(id)->state();
    if( state == layerStateOff ) state = layerStateOn;
    else if( state == layerStateOn ) state = layerStateEdit;
    else state = layerStateOff;
    SdEnvir::instance()->layerGet(id)->stateSet( state );
    //Change in table
    QTableWidgetItem *item = ui->mLayerList->item(row,column);
    if( item != nullptr )
      item->setText( layerState(SdEnvir::instance()->layerGet(id)) );
    item = ui->mLayerList->item( row, 3 );
    if( item != nullptr )
      item->setIcon( QIcon( SdEnvir::instance()->layerGet(id)->isVisible() ? QString(":/pic/lampEnable.png") : QString(":/pic/lampDisable.png") ) );
    }
  else if( column == 3 ) {
    //Icon for visible state. Press on icon change visible state
    SdLayerState state = SdEnvir::instance()->layerGet(id)->state();
    if( state == layerStateOff ) state = layerStateOn;
    else state = layerStateOff;
    SdEnvir::instance()->layerGet(id)->stateSet( state );
    //Change in table
    QTableWidgetItem *item = ui->mLayerList->item(row,2);
    if( item != nullptr )
      item->setText( layerState(SdEnvir::instance()->layerGet(id)) );
    //Change visibility
    item = ui->mLayerList->item( row, 3 );
    if( item != nullptr )
      item->setIcon( QIcon( SdEnvir::instance()->layerGet(id)->isVisible() ? QString(":/pic/lampEnable.png") : QString(":/pic/lampDisable.png") ) );
    }
  else if( column == 4 ) {
    //Layer color
    QColor color = QColorDialog::getColor( SdEnvir::instance()->layerGet(id)->color(), this, tr("Select layer color") );
    if( color.isValid() ) {
      SdEnvir::instance()->layerGet(id)->colorSet( color.rgba() );
      //Change in table
      QTableWidgetItem *item = ui->mLayerList->item(row,column);
      if( item != nullptr ) {
        item->setBackground( color );
        }
      }
    }
  else if( column == 5 ) {
#if 0 //Disable pair changing
    //Layer pair
    QString pair = SdEnvir::instance()->layerGet(id)->pair()->id();
    //If pair not assigned then clear pair id
    if( pair == id )
      pair.clear();
    SdDLayerList pairList( pair, this );
    int r = pairList.exec();
    if( r == SdDLayerList_remove )
      //Remove pair
      SdEnvir::instance()->layerGet(id)->setPair( nullptr );
    else if( r == SdDLayerList_assign ) {
      //Get selected pair
      pair = pairList.pair();
      if( !pair.isEmpty() )
        SdEnvir::instance()->layerGet(id)->setPair( SdEnvir::instance()->layerGet(pair) );
      }
    //Replace pair name in visual representation
    QTableWidgetItem *item = ui->mLayerList->item(row,column);
    if( item != nullptr )
      item->setText( SdEnvir::instance()->layerGet(id)->pair()->name() );
#endif
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
      SdEnvir::instance()->layerGet(id)->nameSet( item->text() );
      }
    }
  }



void SdDLayers::onStratumCountChange(int newCount)
  {
  newCount <<= 1;
  ui->mStratumCountText->setText( QString::number(newCount) );

  mStratumMask = (stmTop|stmBottom);
  newCount -= 2;
  for( int i = 0; i < newCount; ++i )
    mStratumMask |= stmInt04 << i;

  fillLayerList();
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
    case layerTraceStencil :  //Layer for stencil apertures
      return tr("stencil");
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
