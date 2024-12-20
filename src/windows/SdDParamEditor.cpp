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
#include "SdConfig.h"
#include "SdDParamEditor.h"
#include "SdDGetObject.h"
#include "objects/SdProject.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdGraphText.h"
#include "SdDHelp.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QTimer>

SdDParamEditor::SdDParamEditor(const QString title, const SdStringMap &map, SdProject *prj, bool editEnable, bool isProject, QWidget *parent ) :
  SdDParamBase( parent ),
  mProject(prj),
  mEditEnable(editEnable)
  {
  setWindowTitle( title );

  mParam = map;

  //stm32
  bool stm32 = map.value( QString("value") ).toLower().contains("stm32");

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
        vbox->addWidget( mValueSelector = new QPushButton( tr("Select value...")) );
        if( isProject )
          vbox->addWidget( mParamFields = new QPushButton( tr("Accum sheet fields")) );
        else
          mParamFields = nullptr;
        if( stm32 && mEditEnable ) {
          vbox->addWidget( mStm32AlterPinAdd = new QPushButton( tr("stm32 pins import...") ) );
          vbox->addWidget( mStm32AlterPinRemove = new QPushButton( tr("stm32 pins remove") ) );
          }
        else mStm32AlterPinAdd = mStm32AlterPinRemove = nullptr;
      lay->addLayout( vbox );
    root->addLayout( lay );

    QDialogButtonBox *box = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help );
    root->addWidget( box );
    connect( box, &QDialogButtonBox::accepted, this, &SdDParamEditor::accept );
    connect( box, &QDialogButtonBox::rejected, this, &SdDParamEditor::reject );
    connect( box->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
      SdDHelp::help( QString("SdDParamEditor.htm"), this );
      } );
  setLayout( root );
  if( mEditEnable ) {
    //Connect signals when edit enabled
    connect( mParamAdd, &QPushButton::clicked, this, &SdDParamEditor::paramAdd );
    connect( mParamAddDefault, &QPushButton::clicked, this, &SdDParamEditor::paramAddDefault );
    connect( mParamDelete, &QPushButton::clicked, this, &SdDParamEditor::paramDelete );
    connect( mParamCopy, &QPushButton::clicked, this, &SdDParamEditor::paramCopy );
    connect( mValueSelector, &QPushButton::clicked, this, &SdDParamEditor::selectValue );
    if( isProject )
      connect( mParamFields, &QPushButton::clicked, this, &SdDParamEditor::paramFields );
    if( stm32 ) {
      connect( mStm32AlterPinAdd, &QPushButton::clicked, this, &SdDParamEditor::stm32AlterPinAdd );
      connect( mStm32AlterPinRemove, &QPushButton::clicked, this, &SdDParamEditor::stm32AlterPinRemove );
      }
    }
  else {
    //Disable buttons when edit disabled
    mParamAdd->setEnabled(false);
    mParamAddDefault->setEnabled(false);
    mParamDelete->setEnabled(false);
    mParamCopy->setEnabled(false);
    mValueSelector->setEnabled(false);
    }

  resize( 600, 400 );
  QTimer::singleShot( 300, this, [this]() {
    fillParams();
    });
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







void SdDParamEditor::stm32AlterPinAdd()
  {
  //Get configuration file
  QString path = QFileDialog::getOpenFileName( this, tr("STM32CubeIde MX configuration file"), QString{}, tr("STM32CubeIde MX Files (*%1)").arg(".ioc") );
  if( !path.isEmpty() ) {
    //Open configuration file
    QFile is(path);
    if( is.open(QIODevice::ReadOnly) ) {
      //Remove previously assigned names
      stm32AlterPinRemove();
      //Read full file into string
      QString content( QString::fromLatin1(is.readAll()) );
      //Split into separate lines
      QStringList fileLines( content.split('\x0a') );
      //Scan configuration file and append defined pin names
      for( auto const &line : qAsConst(fileLines) ) {
        //Get next line of configuration file
        //Check if there is pin definition
        if( line.contains(".Signal=") || line.contains(".GPIO_Label=") ) {
          //Pin definition presents
          QStringList list = line.split( QChar('=') );
          if( list.count() == 2 ) {
            //Append new pin definition
            paramAddInt( QString("{%1}").arg(list.at(0)), list.at(1) );
            }
          }
        }
      }
    }
  }




void SdDParamEditor::stm32AlterPinRemove()
  {
  for( int row = 0; row < mParamTable->rowCount(); ) {
    QString key = mParamTable->item( row, 0 )->text();
    if( key.contains(".Signal") || key.contains(".GPIO_Label") ) {
      mParam.remove( mParamTable->item( row, 0 )->text() );
      mParamTable->removeRow( row );
      }
    else row++;
    }
  }



