#include "SdM3dFunInput.h"
#include "SdM3dParser.h"

SdM3dFunInput::SdM3dFunInput(char resultType, QTableWidget *tableWidget, int itemType) :
  SdM3dFunction( resultType, SDM3D_TYPE_STRING, resultType ),
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
    mTableWidget->setItem( mRow, 1, item = new QTableWidgetItem(itemType) );
    switch( itemType ) {
      default:
        item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsEditable );
        break;

      case SDM3D_INPUT_BOOL :
        item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
        break;

      case SDM3D_INPUT_PAD :
      case SDM3D_INPUT_COLOR :
        item->setFlags( Qt::ItemIsEnabled );
        break;
      }
    }
  }
