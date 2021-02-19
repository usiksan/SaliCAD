#include "SdM3dFunInput.h"

SdM3dFunInput::SdM3dFunInput(char resultType, QTableWidget *tableWidget) :
  SdM3dFunction( resultType, 2, mInputParamTypes ),
  mTableWidget(tableWidget)
  {
  mInputParamTypes[0] = SDM3D_TYPE_STRING;
  mInputParamTypes[1] = resultType;

  if( mTableWidget != nullptr ) {
    //Row of param
    mRow = mTableWidget->rowCount();
    //Insert row for this input function
    mTableWidget->insertRow(mRow);
    //Setup row height
    mTableWidget->setRowHeight( mRow, 25 );
    //Append title function cell
    mTableWidget->setItem( mRow, 0, new QTableWidgetItem() );
    //Append value function cell
    mTableWidget->setItem( mRow, 1, new QTableWidgetItem() );
    }
  }
