/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Visual Editor for inheritance
*/
#include "SdWEditorInheritance.h"
#include "SdWEditorComponent.h"
#include "SdWEditorGraphSymbol.h"
#include "SdWEditorGraphPart.h"
#include "SdDGetObject.h"
#include "SdDParamEditor.h"
#include "SdDParamDefault.h"
#include "SdDVariantTable.h"
#include "SdWCommand.h"
#include "objects/SdProject.h"
#include "objects/SdPartVariant.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdGraphPartPin.h"

#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>
#include <QTimer>
#include <QDebug>

SdWEditorInheritance::SdWEditorInheritance(SdPItemInheritance *inh, QWidget *parent) :
  SdWEditor( parent ),
  mInheritance(inh),
  mComponent(nullptr)
  {
  mUndo = mInheritance->getUndo();

  // symbol preview         part preview
  //    component name and selection
  //                        param editor
  //                      (param table, buttons)
  QVBoxLayout *root = new QVBoxLayout();

    //Top row contains preview widgets
    QHBoxLayout *lay = new QHBoxLayout();
      //Symbol
      QGroupBox *preview = new QGroupBox( tr("Symbol preview") );
        QHBoxLayout *hbox = new QHBoxLayout();
        preview->setLayout( hbox );
        hbox->addWidget( mSymbolViewer = new SdWEditorGraphView( nullptr, preview ) );
      lay->addWidget( preview );

      //Part
      preview = new QGroupBox( tr("Part preview") );
        hbox = new QHBoxLayout();
        preview->setLayout( hbox );
        hbox->addWidget( mPartViewer = new SdWEditorGraphView( nullptr, preview ) );
      lay->addWidget( preview );
    root->addLayout( lay );

    //Middle row contains component title and component selection button
    hbox = new QHBoxLayout();
      hbox->addWidget( new QLabel(tr("Component:")) );
      hbox->addWidget( mComponentTitle = new QLineEdit() );
      hbox->addWidget( mComponentSelect = new QPushButton( tr("Select component")) );
    root->addLayout( hbox );

    //Bottom row contains edit widgets
    lay = new QHBoxLayout();

      //Component param table
      QVBoxLayout *vbox = new QVBoxLayout();
        vbox->addWidget( new QLabel(tr("Component param table")) );
        vbox->addWidget( mComponentParamTable = new QTableWidget() );
      lay->addLayout( vbox );

      //Buttons
      vbox = new QVBoxLayout();
        vbox->addWidget( mParamInherit = new QPushButton( tr("Inherit param")) );
        vbox->addWidget( mParamInheritAll = new QPushButton( tr("Inherit all params")) );
      lay->addLayout( vbox );


      //Inheritance param table
      vbox = new QVBoxLayout();
        vbox->addWidget( new QLabel(tr("Inheritance param table")) );
        vbox->addWidget( mParamTable = new QTableWidget() );
      lay->addLayout( vbox );

      //Buttons
      vbox = new QVBoxLayout();
        vbox->addWidget( mParamAdd = new QPushButton( tr("Add param")) );
        vbox->addWidget( mParamAddDefault = new QPushButton( tr("Add defaults")) );
        vbox->addWidget( mParamCopy = new QPushButton( tr("Copy param")) );
        vbox->addWidget( mParamDelete = new QPushButton( tr("Delete param")) );
        vbox->addWidget( mParamVariant = new QPushButton( tr("Variant table...")) );
      lay->addLayout( vbox );
    root->addLayout( lay );

  setLayout( root );
  connect( mParamVariant, &QPushButton::clicked, this, &SdWEditorInheritance::paramVariant );
  if( mInheritance->isEditEnable() ) {
    //Connect signals when edit enabled
    connect( mComponentSelect, &QPushButton::clicked, this, &SdWEditorInheritance::componentSelect );

    connect( mParamInherit, &QPushButton::clicked, this, &SdWEditorInheritance::paramInherit );
    connect( mParamInheritAll, &QPushButton::clicked, this, &SdWEditorInheritance::paramInheritAll );


    connect( mParamAdd, &QPushButton::clicked, this, &SdWEditorInheritance::paramAdd );
    connect( mParamAddDefault, &QPushButton::clicked, this, &SdWEditorInheritance::paramAddDefault );
    connect( mParamDelete, &QPushButton::clicked, this, &SdWEditorInheritance::paramDelete );
    connect( mParamCopy, &QPushButton::clicked, this, &SdWEditorInheritance::paramCopy );
    }
  else {
    //Disable buttons when edit disabled
    mComponentSelect->setEnabled(false);

    mParamInherit->setEnabled(false);
    mParamInheritAll->setEnabled(false);

    mParamAdd->setEnabled(false);
    mParamAddDefault->setEnabled(false);
    mParamDelete->setEnabled(false);
    mParamCopy->setEnabled(false);
    }

  QTimer::singleShot( 300, this, [this]() {
    fillComponent();
    fillParams();
    });
  }




SdWEditorInheritance::~SdWEditorInheritance()
  {
  if( mComponent )
    delete mComponent;
  }




SdProjectItem *SdWEditorInheritance::getProjectItem() const
  {
  return mInheritance;
  }




void SdWEditorInheritance::onActivateEditor()
  {
  SdWEditor::onActivateEditor();

  //Activate menu
  SdWCommand::cmObjectEditEnable->setVisible( !mInheritance->isEditEnable() );
  SdWCommand::cmObjectEditDisable->setVisible( mInheritance->isEditEnable() );
  }




void SdWEditorInheritance::cmEditUndo()
  {
  //Refill all visual elements
  fillComponent();
  fillParams();
  dirtyProject();
  }




void SdWEditorInheritance::cmEditRedo()
  {
  cmEditUndo();
  }




void SdWEditorInheritance::onParamChanged(int row, int column)
  {
  Q_UNUSED(column)
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorInheritance::onParamChanged );
  QString key = mParamTable->item(row,0)->text();
  QString value = mParamTable->item(row,1)->text();
  //qDebug() << Q_FUNC_INFO << key <<value;
  mInheritance->paramSet( key, value, mUndo );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorInheritance::onParamChanged );
  updateUndoRedoStatus();
  dirtyProject();
  }




void SdWEditorInheritance::componentSelect()
  {
  QString uid = SdDGetObject::getObjectUid( dctComponent, tr("Select base component for inheritance"), this );
  if( !uid.isEmpty() ) {
    mInheritance->setComponentUid( uid, mUndo );
    fillComponent();
    }
  }




//Copy current param from component param table to inheritance param table
void SdWEditorInheritance::paramInherit()
  {
  int srcRow = mComponentParamTable->currentRow();
  if( srcRow >= 0 ) {
    //Correct row selected
    //Get key and value from source
    QString key = mComponentParamTable->item( srcRow, 0 )->text();
    QString value = mComponentParamTable->item( srcRow, 1 )->text();
    //Insert key and value to dest
    mInheritance->paramSet( key, value, mUndo );
    //Update visual table
    fillParams();
    }
  else
    QMessageBox::warning( this, tr("Error!"), tr("No component parametr selected. Select one and try again.") );
  }





void SdWEditorInheritance::paramInheritAll()
  {
  if( mComponent ) {
    SdStringMap param = mComponent->paramTable(); //Params to get in
    //Append params
    for( auto iter = param.cbegin(); iter != param.cend(); iter++ )
      mInheritance->paramSet( iter.key(), iter.value(), mUndo );
    dirtyProject();
    fillParams();
    }
  else
    QMessageBox::warning( this, tr("Error!"), tr("No component defined for this inheritance or component can't be loaded") );
  }




void SdWEditorInheritance::paramAdd()
  {
  QString key = QInputDialog::getText( this, tr("Param name"), tr("Enter param name") );
  if( !key.isEmpty() ) {
    if( mInheritance->paramContains(key) )
      QMessageBox::warning( this, tr("Warning!"), tr("Param with this name already exist. Enter another name.") );
    else
      paramAddInt( key );
    }
  }





void SdWEditorInheritance::paramAddDefault()
  {
  SdDParamDefault def( this );
  if( def.exec() ) {
    QStringList paramList = def.defParamList();
    for( const QString &param : paramList )
      if( !mInheritance->paramContains(param) )
        paramAddInt( param, SdDParamEditor::defParamValue(param, mInheritance, this) );
    }
  }





void SdWEditorInheritance::paramDelete()
  {
  int paramIndex = mParamTable->currentRow();
  if( paramIndex >= 0 ) {
    QString key = mParamTable->item( paramIndex, 0 )->text();
    mInheritance->paramDelete( key, mUndo );
    mParamTable->removeRow( paramIndex );
    dirtyProject();
    }
  else
    QMessageBox::warning( this, tr("Warning!"), tr("Select param to delete") );
  }




void SdWEditorInheritance::paramCopy()
  {
  SdStringMap param; //Params to get in
  SdPItemComponent *comp = SdDGetObject::getComponent( nullptr, &param, tr("Select component to copy param from"), this );
  if( comp != nullptr ) {
    //Append params
    for( auto iter = param.cbegin(); iter != param.cend(); iter++ )
      mInheritance->paramSet( iter.key(), iter.value(), mUndo );
    dirtyProject();
    fillParams();
    }
  }





void SdWEditorInheritance::paramVariant()
  {
  SdDVariantTable var( mInheritance, mInheritance->isEditEnable(), this );
  var.exec();
  }





void SdWEditorInheritance::fillComponent()
  {
  //Delete previously loaded component
  if( mComponent )
    delete mComponent;
  //Fill component title
  mComponentTitle->setText( mInheritance->componentTitle() );
  //Load component to show schematic and part
  mComponent = sdObjectOnly<SdPItemComponent>( SdObjectFactory::extractObject(mInheritance->componentUid(), false, this ) );
  if( mComponent ) {
    //Component extracted successfully

    //Assign to views schematic and part
    mSymbolViewer->setItemById( mComponent->getSectionSymbolId(0) );
    mPartViewer->setItemById( mComponent->getPartId() );

    //Fill component param table
    mComponentParamTable->clear();
    SdStringMap tab = mComponent->paramTable();
    mComponentParamTable->setColumnCount(2);
    mComponentParamTable->setRowCount( tab.count() );
    //Setup header
    mComponentParamTable->setHorizontalHeaderLabels( {tr("Param name"), tr("Param value")} );
    mComponentParamTable->setColumnWidth(1,400);
    int row = 0;
    for( auto iter = tab.constBegin(); iter != tab.constEnd(); iter++ ) {
      mComponentParamTable->setRowHeight( row, 25 );
      mComponentParamTable->setItem( row, 0, new QTableWidgetItem(iter.key()) );
      mComponentParamTable->item( row, 0 )->setFlags( Qt::ItemIsEnabled );
      mComponentParamTable->setItem( row, 1, new QTableWidgetItem(iter.value()) );
      row++;
      }
    }
  else {
    //Clear views
    mSymbolViewer->setItem( nullptr, false );
    mPartViewer->setItem( nullptr, false );
    //Clear param table
    mComponentParamTable->clear();
    }
  }





void SdWEditorInheritance::fillParams()
  {
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorInheritance::onParamChanged );
  mParamTable->clear();
  SdStringMap tab = mInheritance->paramTable();
  mParamTable->setColumnCount(2);
  mParamTable->setRowCount( tab.count() );
  //Setup header
  mParamTable->setHorizontalHeaderLabels( {tr("Param name"), tr("Param value")} );
  mParamTable->setColumnWidth(1,400);
  int row = 0;
  for( auto iter = tab.constBegin(); iter != tab.constEnd(); iter++ )
    paramAppend( row++, iter.key(), iter.value() );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorInheritance::onParamChanged );
  }





void SdWEditorInheritance::paramAppend(int row, const QString key, const QString value)
  {
  mParamTable->setRowHeight( row, 25 );
  QTableWidgetItem *item = new QTableWidgetItem(key);
  mParamTable->setItem( row, 0, item );
  item->setToolTip( SdDParamEditor::defParamDescription(key) );
  item->setFlags( Qt::ItemIsEnabled );
  mParamTable->setItem( row, 1, new QTableWidgetItem(value) );
  }




void SdWEditorInheritance::paramAddInt(const QString key, const QString value)
  {
  mInheritance->paramSet( key, value, mUndo );
  int row = mParamTable->rowCount();
  mParamTable->insertRow( row );
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorInheritance::onParamChanged );
  paramAppend( row, key, value );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorInheritance::onParamChanged );
  dirtyProject();
  }

