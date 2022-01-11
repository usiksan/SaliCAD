/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds and binds input field for float
*/
#include "SdM3dFunInputFloat.h"
#include "SdM3dParser.h"

SdM3dFunInputFloat::SdM3dFunInputFloat( QTableWidget *tableWidget ) :
  SdM3dFunInput( SD_SCRIPT_TYPE_FLOAT, tableWidget, SDM3D_INPUT_STRING )
  {

  }


float SdM3dFunInputFloat::toFloat() const
  {
  //If none table widget than return default value
  if( mTableWidget == nullptr )
    return mParamList[1]->toFloat();

  //Setup default value
  if( mTableWidget->item( mRow, 1 )->text().isEmpty() ) {
    //Setup input title
    mTableWidget->item( mRow, 0 )->setText( mParamList[0]->toString() );
    //Setup default value
    mTableWidget->item( mRow, 1 )->setText( QString::number( mParamList[1]->toFloat() ) );
    return mParamList[1]->toFloat();
    }

  //Return user entered value
  return mTableWidget->item( mRow, 1 )->text().toFloat();
  }
