#include "SdM3dFunInputColor.h"

SdM3dFunInputColor::SdM3dFunInputColor(QTableWidget *tableWidget) :
  SdM3dFunInput( SDM3D_TYPE_COLOR, tableWidget )
  {

  }


QColor SdM3dFunInputColor::toColor() const
  {
  //If none table widget than return default value
  if( mTableWidget == nullptr )
    return mParamList[1]->toColor();

  //Setup default value
  if( mTableWidget->item( mRow, 1 )->text().isEmpty() ) {
    //Setup input title
    mTableWidget->item( mRow, 0 )->setText( mParamList[0]->toString() );
    //Setup default value
    mTableWidget->item( mRow, 1 )->setText( mParamList[1]->toColor().name() );
    return mParamList[1]->toColor();
    }

  //Return user entered value
  return QColor( mTableWidget->item( mRow, 1 )->text() );
  }
