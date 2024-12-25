/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base dialog for param editing
*/
#include "SdDParamBase.h"
#include "SdDStringFromList.h"
#include "SdValueSelector.h"
#include "SdDParamDefault.h"
#include "SdDGetObject.h"
#include "objects/SdPItemVariant.h"

#include <QInputDialog>
#include <QMessageBox>


SdDParamBase::SdDParamBase(QWidget *parent)
  : QDialog(parent)
  {

  }



//Default params
QStringList SdDParamBase::defParamList()
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
QString SdDParamBase::defParamDescription(QString paramName)
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
QString SdDParamBase::defParamValue(QString paramName, SdProjectItem *item, QWidget *parent)
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




void SdDParamBase::onParamChanged(int row, int column)
  {
  Q_UNUSED(column)
  QString key = mParamTable->item(row,0)->text();
  QString value = mParamTable->item(row,1)->text();
  //qDebug() << Q_FUNC_INFO << key <<value;
  mParam.insert( key, value );
  }




void SdDParamBase::paramAdd()
  {
  QString key = QInputDialog::getText( this, tr("Param name"), tr("Enter param name") );
  if( !key.isEmpty() ) {
    if( mParam.contains(key) )
      QMessageBox::warning( this, tr("Warning!"), tr("Param with this name already exist. Enter another name.") );
    else
      paramAddInt( key );
    }
  }




void SdDParamBase::paramAddDefault()
  {
  SdDParamDefault def( this );
  if( def.exec() ) {
    QStringList paramList = def.defParamList();
    for( const QString &param : paramList )
      if( !mParam.contains(param) )
        paramAddInt( param, defParamValue(param, nullptr, this) );
    }
  }




void SdDParamBase::paramDelete()
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



void SdDParamBase::paramCopy()
  {
  SdStringMap param; //Params to get in
  SdPItemVariant *comp = SdDGetObject::getComponent( nullptr, &param, tr("Select component to copy param from"), this );
  if( comp != nullptr ) {
    //Append params
    for( auto iter = param.cbegin(); iter != param.cend(); iter++ )
      mParam.insert( iter.key(), iter.value() );
    fillParams();
    delete comp;
    }
  }





void SdDParamBase::selectValue()
  {
  if( SdValueSelector::select( &mParam, this ) ) {
    //Value updated
    fillParams();
    }
  }




void SdDParamBase::fillParams()
  {
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamBase::onParamChanged );
  mParamTable->clear();
  mParamTable->setColumnCount(2);
  mParamTable->setRowCount( mParam.count() );
  //Setup header
  mParamTable->setHorizontalHeaderLabels( {tr("Param name"), tr("Param value")} );
  mParamTable->setColumnWidth(1,200);
  int row = 0;
  for( auto iter = mParam.constBegin(); iter != mParam.constEnd(); iter++ )
    paramAppend( row++, iter.key(), iter.value() );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamBase::onParamChanged );
  }




void SdDParamBase::paramAppend(int row, const QString key, const QString value)
  {
  mParamTable->setRowHeight( row, 25 );
  QTableWidgetItem *item = new QTableWidgetItem(key);
  mParamTable->setItem( row, 0, item );
  item->setToolTip( SdDParamBase::defParamDescription(key) );
  item->setFlags( Qt::ItemIsEnabled );
  mParamTable->setItem( row, 1, new QTableWidgetItem(value) );
  }




void SdDParamBase::paramAddInt( const QString key, const QString value )
  {
  mParam.insert( key, value );
  int row = mParamTable->rowCount();
  mParamTable->insertRow( row );
  disconnect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamBase::onParamChanged );
  paramAppend( row, key, value );
  connect( mParamTable, &QTableWidget::cellChanged, this, &SdDParamBase::onParamChanged );
  }
