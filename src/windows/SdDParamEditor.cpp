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
#include "SdDStringFromList.h"
#include "SdValueSelector.h"
#include "SdDParamDefault.h"
#include "objects/SdProject.h"
#include "objects/SdPItemComponent.h"
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
  QDialog( parent ),
  mProject(prj),
  mParam(map),
  mEditEnable(editEnable)
  {
  setWindowTitle( title );

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






//Default params
QStringList SdDParamEditor::defParamList()
  {
  static QStringList list {
    stdParamBom,
    stdParamArticle,
    stdParamTitle,
    stdParamValue,
    stdParamValueMin,
    stdParamValueMax,
    stdParamValueRow,
    stdParamPrefix,
    stdParamValueSelector,
    stdParamManufacturer,
    stdParamTolerance,
    stdParamDatasheet,
    stdParamWww,
    stdParamLogNumber,
    stdParamItemId,
    stdParamItemIdList,
    stdParamCompCount,
    stdParamTotalPageCount,
    stdParamPageIndex,
    stdParamDeviceMark,
    stdParamCategory,
    //Capacitor specific
    stdParamDielectric,
    stdParamVoltage,
    //Diod specific
    stdParamForvardVoltage,
    stdParamCurent,
    stdParamReverceVoltage,
    //LED specific
    stdParamAngle,
    stdParamBrightness,
    stdParamColor,
    stdParamWavelength,
    //Inductor specific
    stdParamSatCurrent,
    stdParamResistance,
    stdParamSRF,
    //MOSFET specific
    stdParamId,
    stdParamPd,
    stdParamRdsOn,
    stdParamVdss,
    stdParamVgs,
    //Transistor specific
    stdParamIc,
    stdParamVcbo,
    stdParamVceo,
    stdParamHFE,
    //3d model
    stdParam3dModelProgramm
    };

  return list;
  }





//Default param description
QString SdDParamEditor::defParamDescription(QString paramName)
  {
  static QMap<QString,QString> map;
  if( map.isEmpty() ) {
    //Create map
    map.insert( stdParamBom, tr("This parameter define component line representation in bill of material report") );
    map.insert( stdParamArticle, tr("Component article representation. Can be used in component name and also in bom") );
    map.insert( stdParamTitle, tr("This parametr define component name without value for example smd 0805") );
    map.insert( stdParamValue, tr("This parameter define concrete component value for example 1kOm") );
    map.insert( stdParamValueMin, tr("Minimum for value parameter, for example 1 Om") );
    map.insert( stdParamValueMax, tr("Maximum for value parameter, for example 10 MOm") );
    map.insert( stdParamValueRow, tr("Row with which forms all intermediate values, for example E12") );
    map.insert( stdParamPrefix, tr("This param define component ident prefix for example prefix DD will construct ident DD4") );
    map.insert( stdParamValueSelector, tr("This param define used value selector for example 'resistor' will select resistor values, i.e. 1.2kOm") );
    map.insert( stdParamManufacturer, tr("Manufacturer of component") );
    map.insert( stdParamTolerance, tr("Tolerance of value") );
    map.insert( stdParamDatasheet, tr("Local path to pdf file with datasheet") );
    map.insert( stdParamWww, tr("www page address") );
    map.insert( stdParamDeviceMark, tr("Device marking code") );
    map.insert( stdParamCategory, tr("Path to element in category tree") );
    //Capacitor specific
    map.insert( stdParamDielectric, tr("Capacitor dielectric type") );
    map.insert( stdParamVoltage, tr("Capacitor working voltage") );
    //Diod specific
    map.insert( stdParamForvardVoltage, tr("Diod forvard voltage") );
    map.insert( stdParamCurent, tr("Forvard continuous current") );
    map.insert( stdParamReverceVoltage, tr("Diod reverce voltage") );
    //LED specific
    map.insert( stdParamAngle, tr("LED angle") );
    map.insert( stdParamBrightness, tr("LED brightness") );
    map.insert( stdParamColor, tr("LED color") );
    map.insert( stdParamWavelength, tr("LED wavelength") );
    //Inductor specific
    map.insert( stdParamSatCurrent, tr("Inductor saturated current") );
    map.insert( stdParamResistance, tr("Inductor resistance") );
    map.insert( stdParamSRF, tr("Inductor self resonant frequency") );
    //MOSFET specific
    map.insert( stdParamId, tr("MOSFET dest current") );
    map.insert( stdParamPd, tr("MOSFET dest power or transistor power") );
    map.insert( stdParamRdsOn, tr("MOSFET source-dest resistance when ON state") );
    map.insert( stdParamVdss, tr("MOSFET source-dest voltage when OFF") );
    map.insert( stdParamVgs, tr("MOSFET gate-source voltage") );
    //Transistor specific
    map.insert( stdParamIc, tr("Transistor collector current") );
    map.insert( stdParamVcbo, tr("Transistor maximum Voltage which it can withstand on its collector measured relative to its base with the emitter open circuit") );
    map.insert( stdParamVceo, tr("Transistor maximum voltage which it can withstand on its collector measured relative to its emitter with the base open circuit") );
    map.insert( stdParamHFE, tr("Transistor current-gain koef") );
    //3d model
    map.insert( stdParam3dModelProgramm, tr("Description for 3d model programm") );
    }
  return map.value( paramName );
  }





//Default param value
QString SdDParamEditor::defParamValue(QString paramName, SdProjectItem *item, QWidget *parent)
  {
  if( paramName == stdParamValueSelector ) {
    //Show list with available
    SdDStringFromList sl( SdValueSelector::availableSelectors(), [] ( QString key ) -> QString {
      return SdValueSelector::shortSelectorDescription(key);
      }, parent );
    if( sl.exec() )
      return sl.result();
    }
  if( paramName == stdParamPrefix ) return QStringLiteral("id");
  if( paramName == stdParamBom ) return QStringLiteral("<article> <title> <value>");
  if( paramName == stdParamTitle ) {
    if( item ) return item->getTitle();
    }
  return QString();
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
  SdDParamDefault def( this );
  if( def.exec() ) {
    QStringList paramList = def.defParamList();
    for( const QString &param : paramList )
      if( !mParam.contains(param) )
        paramAddInt( param, defParamValue(param, nullptr, this) );
    }
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





void SdDParamEditor::selectValue()
  {
  if( SdValueSelector::select( &mParam, this ) ) {
    //Value updated
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

  //Enable value selector
  if( key == stdParamValueSelector && mParamCopy->isEnabled() )
    mValueSelector->setEnabled(true);
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








