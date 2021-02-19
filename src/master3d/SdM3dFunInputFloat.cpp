#include "SdM3dFunInputFloat.h"

SdM3dFunInputFloat::SdM3dFunInputFloat( QTableWidget *tableWidget ) :
  SdM3dFunInput( SDM3D_TYPE_FLOAT, tableWidget )
  {

  }


float SdM3dFunInputFloat::toFloat() const
  {
  //If none table widget than return default value
  if( mTableWidget == nullptr )
    return mParamList[1]->toFloat();

  //Setup input title
  mTableWidget->item( mRow, 0 )->setText( mParamList[0]->toString() );
  //Setup default value
  if( mTableWidget->item( mRow, 1 )->text().isEmpty() ) {
    mTableWidget->item( mRow, 1 )->setText( QString::number( mParamList[1]->toFloat() ) );
    return mParamList[1]->toFloat();
    }

  //Return user entered value
  return mTableWidget->item( mRow, 1 )->text().toFloat();
  }
