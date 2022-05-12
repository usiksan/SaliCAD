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

  SdExpressionRefMap derived class from SdScriptRefMap represents map which associates key and value.
  Key is name of variable or param.  Value is value of variable or param.

  SdExpressionRefMap represents map for schematic calculations
*/
#include "SdExpressionRefMap.h"

SdExpressionRefMap::SdExpressionRefMap(QTableWidget *table) :
  mTable(table)
  {

  }





//!
//! \brief parseEnd Call at end of parsing to reorganize visual table of variables
//!
void SdExpressionRefMap::parseEnd()
  {
  //In refMap leave variables which must be deleted from visual table
  for( auto it = mRefMap.cbegin(); it != mRefMap.cend(); it++ ) {
    QString key = it.key();
    //Find index in table which must be deleted
    int index;
    for( index = 0; index < mTable->rowCount(); index++ )
      if( mRefList.at(index) == key ) break;
    if( index < mTable->rowCount() ) {
      //Delete from internal list
      mRefList.removeAt( index );
      //Delete from visual list
      mTable->removeRow( index );
      }
    }

  //Insert in visual list newly appended variables
  int i = mTable->rowCount();
  mTable->setRowCount( mRefList.count() );
  for( ; i < mRefList.count(); i++ ) {
    mTable->setRowHeight( i, 15 );
    //Variable name
    auto item = new QTableWidgetItem( mRefList.at(i) );
    item->setFlags( Qt::ItemIsEnabled );
    mTable->setItem( i, 0, item );

    //Variable value
    item = new QTableWidgetItem();
    item->setFlags( Qt::ItemIsEditable | Qt::ItemIsEnabled );
    mTable->setItem( i, 1, item );
    }

  //Refill map to speeding up variable access
  mRefMap.clear();
  for( int i = 0; i < mRefList.count(); i++ )
    mRefMap.insert( mRefList.at(i), i );
  }




//!
//! \brief varInit Appends key into map
//! \param key     Appended key
//!
//! Overrided function. It append key to mRefList if it yet appended
void SdExpressionRefMap::varInit(const QString &key)
  {
  if( mRefMap.contains(key) ) {
    //Remove variable from delete list
    mRefMap.remove(key);
    }
  else {
    //Append new variable to list
    mRefList.append(key);
    }
  }




//!
//! \brief varGet Returns value associated with this key
//! \param key    Key for which value must be returned
//! \return       Value associated with this key
//!
//! Overrided function. It extract value of key from visual table
QString SdExpressionRefMap::varGet(const QString &key) const
  {
  if( mRefMap.contains(key) )
    return mTable->item( mRefMap.value( key ), 1 )->text();
  return QString("0");
  }




//!
//! \brief varSet Replace previously value associated with key with new value
//! \param key    Key for which value must be replaced
//! \param val    New value associated with key
//!
//! Overrided function. It replace value associated with key with new value
//! in visual table
void SdExpressionRefMap::varSet(const QString &key, const QString &val)
  {
  if( mRefMap.contains(key) )
    mTable->item( mRefMap.value( key ), 1 )->setText( val );
  }
