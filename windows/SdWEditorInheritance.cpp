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
      lay->addLayout( vbox );
    root->addLayout( lay );

  setLayout( root );
  if( mComponent->isEditEnable() ) {
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
    mParamDelete->setEnabled(false);
    mParamCopy->setEnabled(false);
    }

  QTimer::singleShot( 300, this, [this]() {
    fillComponent();
    fillParams();
    });
  }




SdProjectItem *SdWEditorInheritance::getProjectItem() const
  {
  return mInheritance;
  }




void SdWEditorInheritance::onActivateEditor()
  {
  SdWEditor::onActivateEditor();

  //Activate menu
  SdWCommand::cmObjectEditEnable->setVisible( !mComponent->isEditEnable() );
  SdWCommand::cmObjectEditDisable->setVisible( mComponent->isEditEnable() );
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
  mComponent->paramSet( key, value, mUndo );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdWEditorInheritance::onParamChanged );
  updateUndoRedoStatus();
  dirtyProject();
  }




void SdWEditorInheritance::componentSelect()
  {

  }

void SdWEditorInheritance::paramInherit()
  {

  }

void SdWEditorInheritance::paramInheritAll()
  {

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

  }

void SdWEditorInheritance::paramDelete()
  {

  }

void SdWEditorInheritance::paramCopy()
  {

  }

void SdWEditorInheritance::fillComponent()
  {

  }

void SdWEditorInheritance::fillParams()
  {

  }

void SdWEditorInheritance::paramAppend(int row, const QString key, const QString value)
  {

  }

void SdWEditorInheritance::paramAddInt(const QString key, const QString value)
  {

  }
