/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for param editing and viewing from container.
*/
#include "SdDParamEditor.h"
#include "SdDGetObject.h"
#include "objects/SdProject.h"
#include "objects/SdPItemComponent.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdGraphText.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QTimer>

SdDParamEditor::SdDParamEditor( const QString title, SdContainer *cnt, bool editEnable, QWidget *parent ) :
  QDialog( parent ),
  mItem(cnt),
  mProject(nullptr),
  mEditEnable(editEnable)
  {
  setWindowTitle( title );

  SdProject *prj = dynamic_cast<SdProject*>( mItem->getRoot() );
  if( prj == nullptr ) prj = mProject = dynamic_cast<SdProject*>( mItem );
  Q_ASSERT( prj != nullptr );
  mUndo = prj->getUndo();

  // symbol preview         part preview
  //    component name and selection
  //                        param editor
  //                      (param table, buttons)
  QVBoxLayout *root = new QVBoxLayout();

    QHBoxLayout *lay = new QHBoxLayout();
      //Contaner param table
      QVBoxLayout *vbox = new QVBoxLayout();
        vbox->addWidget( new QLabel(tr("Param table")) );
        vbox->addWidget( mParamTable = new QTableWidget() );
      lay->addLayout( vbox );

      //Buttons
      vbox = new QVBoxLayout();
        vbox->addWidget( mParamAdd = new QPushButton( tr("Add param")) );
        vbox->addWidget( mParamAddDefault = new QPushButton( tr("Add defaults")) );
        vbox->addWidget( mParamCopy = new QPushButton( tr("Copy param")) );
        vbox->addWidget( mParamDelete = new QPushButton( tr("Delete param")) );
        if( mProject )
          vbox->addWidget( mParamFields = new QPushButton( tr("Accum sheet fields")) );
        else
          mParamFields = nullptr;
      lay->addLayout( vbox );
    root->addLayout( lay );

    QDialogButtonBox *box = new QDialogButtonBox( QDialogButtonBox::Ok );
    root->addWidget( box );
    connect( box, &QDialogButtonBox::accepted, this, &SdDParamEditor::accept );
  setLayout( root );
  if( mEditEnable ) {
    //Connect signals when edit enabled
    connect( mParamAdd, &QPushButton::clicked, this, &SdDParamEditor::paramAdd );
    connect( mParamAddDefault, &QPushButton::clicked, this, &SdDParamEditor::paramAddDefault );
    connect( mParamDelete, &QPushButton::clicked, this, &SdDParamEditor::paramDelete );
    connect( mParamCopy, &QPushButton::clicked, this, &SdDParamEditor::paramCopy );
    if( mProject )
      connect( mParamFields, &QPushButton::clicked, this, &SdDParamEditor::paramFields );
    }
  else {
    //Disable buttons when edit disabled
    mParamAdd->setEnabled(false);
    mParamAddDefault->setEnabled(false);
    mParamDelete->setEnabled(false);
    mParamCopy->setEnabled(false);
    }

  resize( 600, 400 );
  QTimer::singleShot( 300, this, [this]() {
    fillParams();
    });
  }




void SdDParamEditor::onParamChanged(int row, int column)
  {
  Q_UNUSED(column)
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamEditor::onParamChanged );
  QString key = mParamTable->item(row,0)->text();
  QString value = mParamTable->item(row,1)->text();
  //qDebug() << Q_FUNC_INFO << key <<value;
  mItem->paramSet( key, value, mUndo );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamEditor::onParamChanged );
  dirtyProject();
  }




void SdDParamEditor::paramAdd()
  {
  QString key = QInputDialog::getText( this, tr("Param name"), tr("Enter param name") );
  if( !key.isEmpty() ) {
    if( mItem->paramContains(key) )
      QMessageBox::warning( this, tr("Warning!"), tr("Param with this name already exist. Enter another name.") );
    else
      paramAddInt( key );
    }
  }




void SdDParamEditor::paramAddDefault()
  {
  if( !mItem->paramContains(QStringLiteral("bom")) )
    paramAddInt( QStringLiteral("bom"), QStringLiteral("<article> <title> <value>") );
  if( !mItem->paramContains(QStringLiteral("article")) )
    paramAddInt( QStringLiteral("article") );
  if( !mItem->paramContains(QStringLiteral("title")) )
    paramAddInt( QStringLiteral("title") );
  if( !mItem->paramContains(QStringLiteral("value")) )
    paramAddInt( QStringLiteral("value") );
  }




void SdDParamEditor::paramDelete()
  {
  int paramIndex = mParamTable->currentRow();
  if( paramIndex >= 0 ) {
    QString key = mParamTable->item( paramIndex, 0 )->text();
    mItem->paramDelete( key, mUndo );
    mParamTable->removeRow( paramIndex );
    dirtyProject();
    }
  else
    QMessageBox::warning( this, tr("Warning!"), tr("Select param to delete") );
  }




void SdDParamEditor::paramCopy()
  {
  SdStringMap param; //Params to get in
  SdPItemComponent *comp = SdDGetObject::getComponent( nullptr, &param, tr("Select component to copy param from"), this );
  if( comp != nullptr ) {
    //Append params
    for( auto iter = param.cbegin(); iter != param.cend(); iter++ )
      mItem->paramSet( iter.key(), iter.value(), mUndo );
    dirtyProject();
    fillParams();
    }
  }





void SdDParamEditor::paramFields()
  {
  if( mProject ) {
    //Scan all sheets
    //In each sheet scan all texts
    //For each text we test if it is field then append it to table
    mProject->forEach( dctSheet, [this] ( SdObject *obj ) -> bool {
      SdPItemSheet *sheet = dynamic_cast<SdPItemSheet*>( obj );
      if( sheet ) {
        sheet->forEach( dctText, [this] (SdObject *obj ) -> bool {
          SdGraphText *text = dynamic_cast<SdGraphText*>( obj );
          if( text ) {
            QString str = text->getText();
            if( str.startsWith( QChar('{')) && str.endsWith( QChar('}')) ) {
              //This text is field. Append it to table
              if( !mItem->paramContains(str) )
                mItem->paramSet( str, str, mUndo );
              }
            }
          return true;
          });
        }
      return true;
      } );
    dirtyProject();
    fillParams();
    }
  }




void SdDParamEditor::fillParams()
  {
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamEditor::onParamChanged );
  mParamTable->clear();
  SdStringMap tab = mItem->paramTable();
  mParamTable->setColumnCount(2);
  mParamTable->setRowCount( tab.count() );
  //Setup header
  mParamTable->setHorizontalHeaderLabels( {tr("Param name"), tr("Param value")} );
  mParamTable->setColumnWidth(1,400);
  int row = 0;
  for( auto iter = tab.constBegin(); iter != tab.constEnd(); iter++ )
    paramAppend( row++, iter.key(), iter.value() );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamEditor::onParamChanged );
  }




void SdDParamEditor::paramAppend(int row, const QString key, const QString value)
  {
  mParamTable->setRowHeight( row, 25 );
  mParamTable->setItem( row, 0, new QTableWidgetItem(key) );
  mParamTable->item( row, 0 )->setFlags( Qt::ItemIsEnabled );
  mParamTable->setItem( row, 1, new QTableWidgetItem(value) );
  }






void SdDParamEditor::paramAddInt(const QString key, const QString value)
  {
  mItem->paramSet( key, value, mUndo );
  int row = mParamTable->rowCount();
  mParamTable->insertRow( row );
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamEditor::onParamChanged );
  paramAppend( row, key, value );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamEditor::onParamChanged );
  dirtyProject();
  }




void SdDParamEditor::dirtyProject()
  {
  //Set project dirty
  SdProject *prj = dynamic_cast<SdProject*>( mItem->getRoot() );
  if( prj ) prj->setDirty();
  }