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

  The function builds and binds input field for color
*/
#include "SdScriptValueFunInputColor.h"
#include "SdScriptParser.h"

SdScriptValueFunInputColor::SdScriptValueFunInputColor(QTableWidget *tableWidget) :
  SdScriptValueFunInput( SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_STRING, tableWidget, SDM3D_INPUT_COLOR )
  {

  }



//!
//! \brief toColor Convert object to color
//! \return        Color
//!
QColor SdScriptValueFunInputColor::toColor() const
  {
  //If none table widget than return default value
  if( mTableWidget == nullptr )
    return QColor( mParamList[1]->toString() );

  //Setup default value
  if( mTableWidget->item( mRow, 1 )->text().isEmpty() ) {
    //Setup input title
    mTableWidget->item( mRow, 0 )->setText( mParamList[0]->toString() );
    //Setup default value
    mTableWidget->item( mRow, 1 )->setText( mParamList[1]->toString() );
    return QColor( mParamList[1]->toString() );
    }

  //Return user entered value
  return QColor( mTableWidget->item( mRow, 1 )->text() );
  }
