/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds and binds input field for pad
*/
#include "SdM3dFunInputPad.h"
#include "SdM3dParser.h"


SdM3dFunInputPad::SdM3dFunInputPad(QTableWidget *tableWidget) :
  SdM3dFunInput( SDM3D_TYPE_STRING, tableWidget, SDM3D_INPUT_PAD )
  {

  }


QString SdM3dFunInputPad::toString() const
  {
  //If none table widget than return default value
  if( mTableWidget == nullptr )
    return mParamList[1]->toString();

  //Setup default value
  if( mTableWidget->item( mRow, 1 )->text().isEmpty() ) {
    //Setup input title
    mTableWidget->item( mRow, 0 )->setText( mParamList[0]->toString() );
    //Setup default value
    mTableWidget->item( mRow, 1 )->setText( mParamList[1]->toString() );
    return mParamList[1]->toString();
    }

  //Return user entered value
  return mTableWidget->item( mRow, 1 )->text();
  }
