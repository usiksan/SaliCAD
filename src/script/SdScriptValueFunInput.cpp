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

  This function appends one field to program param table
*/
#include "SdScriptValueFunInput.h"
#include "SdScriptParser.h"

SdScriptValueFunInput::SdScriptValueFunInput(char resultType, char defaultType, QTableWidget *tableWidget, int itemType) :
  SdScriptValueFunction( resultType, SD_SCRIPT_TYPE_STRING, defaultType ),
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
