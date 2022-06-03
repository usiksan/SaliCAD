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

  The function builds and binds input field for bool
*/
#include "SdScriptValueFunInputBool.h"
#include "SdScriptParser.h"

SdScriptValueFunInputBool::SdScriptValueFunInputBool(QTableWidget *tableWidget) :
  SdScriptValueFunInput( SD_SCRIPT_TYPE_BOOL, SD_SCRIPT_TYPE_BOOL, tableWidget, SDM3D_INPUT_BOOL )
  {

  }


bool SdScriptValueFunInputBool::toBool() const
  {
  //If none table widget than return default value
  if( mTableWidget == nullptr )
    return mParamList[1]->toBool();

  //Setup default value
  if( mTableWidget->item( mRow, 1 )->text().isEmpty() ) {
    //Setup input title
    mTableWidget->item( mRow, 0 )->setText( mParamList[0]->toString() );
    //Setup default value
    mTableWidget->item( mRow, 1 )->setCheckState( mParamList[1]->toBool() ? Qt::Checked : Qt::Unchecked );
    return mParamList[1]->toBool();
    }

  //Return user entered value
  return mTableWidget->item( mRow, 1 )->checkState() == Qt::Checked;
  }
