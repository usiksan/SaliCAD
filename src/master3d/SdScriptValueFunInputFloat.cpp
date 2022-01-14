/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Embedded function.

  The function builds and binds input field for float
*/
#include "SdScriptValueFunInputFloat.h"
#include "SdM3dParser.h"

SdScriptValueFunInputFloat::SdScriptValueFunInputFloat( QTableWidget *tableWidget ) :
  SdScriptValueFunInput( SD_SCRIPT_TYPE_FLOAT, tableWidget, SDM3D_INPUT_STRING )
  {

  }




//!
//! \brief toFloat Convert object ot float value
//! \return        float value
//!
float SdScriptValueFunInputFloat::toFloat() const
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
