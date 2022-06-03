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

  The function builds and binds input field for pad
*/
#include "SdScriptValueFunInputPad.h"
#include "SdScriptParser.h"


SdScriptValueFunInputPad::SdScriptValueFunInputPad(QTableWidget *tableWidget) :
  SdScriptValueFunInput( SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_STRING, tableWidget, SDM3D_INPUT_PAD )
  {

  }




//!
//! \brief toString Convert object to string
//! \return         String
//!
QString SdScriptValueFunInputPad::toString() const
  {
  //If none table widget than return default value
  if( mTableWidget == nullptr )
    return mParamList[1]->toString();

  //Setup default value
  if( mTableWidget->item( mRow, 1 )->text().isEmpty() ) {
    //Setup input title
    mTableWidget->item( mRow, 0 )->setText( mParamList[0]->toString() );
    //Setup default value
    mTableWidget->item( mRow, 1 )->setStatusTip( mParamList[1]->toString() );
    mTableWidget->item( mRow, 1 )->setText( mParamList[1]->toString() );
    return mParamList[1]->toString();
    }

  //If value not edited then return default value
  if( mTableWidget->item( mRow, 1 )->text() == mTableWidget->item( mRow, 1 )->statusTip() ) {
    //Value not edited, replace default value
    mTableWidget->item( mRow, 1 )->setStatusTip( mParamList[1]->toString() );
    mTableWidget->item( mRow, 1 )->setText( mParamList[1]->toString() );
    return mParamList[1]->toString();
    }

  //Return user entered value
  return mTableWidget->item( mRow, 1 )->text();
  }
