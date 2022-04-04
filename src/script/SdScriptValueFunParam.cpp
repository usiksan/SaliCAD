#include "SdScriptValueFunParam.h"


SdScriptValueFunParam::SdScriptValueFunParam(QTableWidget *tableWidget) :
  SdScriptValueFunction( SD_SCRIPT_TYPE_PARAM, SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_STRING ),
  mTableWidget(tableWidget)
  {
  if( mTableWidget != nullptr ) {
    //Row of param
    mRow = mTableWidget->rowCount();
    //Insert row for this input function
    mTableWidget->insertRow(mRow);
    //Setup row height
    mTableWidget->setRowHeight( mRow, 25 );
    //Append title function cell
    QTableWidgetItem *item;
    mTableWidget->setItem( mRow, 0, item = new QTableWidgetItem() );
    item->setFlags( Qt::ItemIsEnabled );
    //Append value function cell
    mTableWidget->setItem( mRow, 1, item = new QTableWidgetItem() );
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsEditable );
    }
  }

void SdScriptValueFunParam::titleSet()
  {
  if( mTableWidget != nullptr )
    mTableWidget->item( mRow, 0 )->setText( mParamList[0]->toString() );
  }



QString SdScriptValueFunParam::toString() const
  {
  if( mTableWidget == nullptr )
    return QString{};

  return mTableWidget->item( mRow, 1 )->text();
  }
