/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for param editing and viewing from given table. All changes are made in local.
  If nessesery result param table apply manualy
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

SdDParamEditor::SdDParamEditor(const QString title, const SdStringMap &map, SdProject *prj, bool editEnable, QWidget *parent ) :
  QDialog( parent ),
  mProject(prj),
  mParam(map),
  mEditEnable(editEnable)
  {
  setWindowTitle( title );

  //  param editor
  // (param table, buttons)
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

    QDialogButtonBox *box = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    root->addWidget( box );
    connect( box, &QDialogButtonBox::accepted, this, &SdDParamEditor::accept );
    connect( box, &QDialogButtonBox::rejected, this, &SdDParamEditor::reject );
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
  mParam.insert( key, value );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamEditor::onParamChanged );
  }




void SdDParamEditor::paramAdd()
  {
  QString key = QInputDialog::getText( this, tr("Param name"), tr("Enter param name") );
  if( !key.isEmpty() ) {
    if( mParam.contains(key) )
      QMessageBox::warning( this, tr("Warning!"), tr("Param with this name already exist. Enter another name.") );
    else
      paramAddInt( key );
    }
  }




void SdDParamEditor::paramAddDefault()
  {
  if( !mParam.contains(QStringLiteral("bom")) )
    paramAddInt( QStringLiteral("bom"), QStringLiteral("<article> <title> <value>") );
  if( !mParam.contains(QStringLiteral("article")) )
    paramAddInt( QStringLiteral("article") );
  if( !mParam.contains(QStringLiteral("title")) )
    paramAddInt( QStringLiteral("title") );
  if( !mParam.contains(QStringLiteral("value")) )
    paramAddInt( QStringLiteral("value") );
  }




void SdDParamEditor::paramDelete()
  {
  int paramIndex = mParamTable->currentRow();
  if( paramIndex >= 0 ) {
    QString key = mParamTable->item( paramIndex, 0 )->text();
    mParam.remove( key );
    mParamTable->removeRow( paramIndex );
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
      mParam.insert( iter.key(), iter.value() );
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
              if( !mParam.contains(str) )
                mParam.insert( str, str );
              }
            }
          return true;
          });
        }
      return true;
      } );
    fillParams();
    }
  }




void SdDParamEditor::fillParams()
  {
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamEditor::onParamChanged );
  mParamTable->clear();
  mParamTable->setColumnCount(2);
  mParamTable->setRowCount( mParam.count() );
  //Setup header
  mParamTable->setHorizontalHeaderLabels( {tr("Param name"), tr("Param value")} );
  mParamTable->setColumnWidth(1,400);
  int row = 0;
  for( auto iter = mParam.constBegin(); iter != mParam.constEnd(); iter++ )
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
  mParam.insert( key, value );
  int row = mParamTable->rowCount();
  mParamTable->insertRow( row );
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamEditor::onParamChanged );
  paramAppend( row, key, value );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamEditor::onParamChanged );
  }








