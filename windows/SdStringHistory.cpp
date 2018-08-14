/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Represent history of strings.
  Support history limitation, recently used
*/
#include "SdStringHistory.h"
#include <QDebug>

SdStringHistory::SdStringHistory(int limit, int precision) :
  mPrecision(precision),
  mLimit(limit)
  {

  }



//Append string to history
int SdStringHistory::addString(const QString str)
  {
  //Test if string already present in history
  int i = indexOf( str );
  if( i >= 0 ) {
    //String present
    if( i > 0 ) {
      //We need move str to first position in history
      //Remove it from current position
      removeAt( i );
      //Insert string to first position
      insert( 0, str );
      }
    }
  else {
    //Test if history is full
    if( count() >= mLimit ) {
      //History is full, remove last item
      i = count() - 1;
      removeAt( i );
      }
    insert( 0, str );
    }
  return i;
  }





//Append string representation of double to history
//This function normalize string to double
//Return index which index must removed from history
int SdStringHistory::addDoubleString(const QString str)
  {
  QString db( str );
  db.replace( QChar(','), QChar('.') );
  return addDouble( db.toDouble() );
  }





//Append double.
//Converted double to string representation and append as string
//Return index which index must removed from history
int SdStringHistory::addDouble(double val)
  {
  return addString( QString::number( val, 'f', mPrecision ) );
  }






//Entered or selected new current string in combo box
//With this fucntion we reorder stringHistory itself and comboBox
void SdStringHistory::reorderComboBoxString(QComboBox *box)
  {
  QString str = box->currentText();
  qDebug() << "reorder" << str;
  int i = addString( str );
  if( i >= 0 )
    box->removeItem( i );
  box->insertItem( 0, str );
  box->setCurrentText( str );
  }





//Entered or selected new current string represented double in combo box
//With this fucntion we reorder stringHistory itself and comboBox
void SdStringHistory::reorderComboBoxDoubleString(QComboBox *box)
  {
  QString str = box->currentText();
  int i = addDoubleString( str );
  if( i >= 0 )
    box->removeItem( i );
  box->insertItem( 0, str );
  box->setCurrentText( str );
  }




