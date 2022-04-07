#include "SdGraphScriptRefMap.h"

SdGraphScriptRefMap::SdGraphScriptRefMap()
  {

  }



SdRect SdGraphScriptRefMap::draw(SdContext *dc)
  {
  drawExcept( dc, -1 );
  //Collect Rectangle

  }



void SdGraphScriptRefMap::parseBegin()
  {
  }



void SdGraphScriptRefMap::parseEnd()
  {
  //In refMap leave variables which must be deleted from visual table
  for( auto it = mRefMap.cbegin(); it != mRefMap.cend(); it++ ) {
    QString key = it.key();
    //Find index in table which must be deleted
    int index;
    for( index = 0; index < mRefList.count(); index++ )
      if( mRefList.at(index).name() == key ) {
        //Delete from internal list
        mRefList.removeAt( index );
        break;
        }
    }

  //Refill map to speeding up variable access
  mRefMap.clear();
  for( int i = 0; i < mRefList.count(); i++ )
    mRefMap.insert( mRefList.at(i).name(), i );
  }



void SdGraphScriptRefMap::varInit(const QString &key)
  {
  if( mRefMap.contains(key) ) {
    //Remove variable from delete list
    mRefMap.remove(key);
    }
  else {
    //Append new variable to list
    mRefList.append( SdGraphScriptRef(key) );
    }
  }





QString SdGraphScriptRefMap::varGet(const QString &key) const
  {
  if( mRefMap.contains(key) )
    return mRefList.at( mRefMap.value(key) ).valueGet();
  return QString{};
  }



void SdGraphScriptRefMap::varSet(const QString &key, const QString &val)
  {
  if( mRefMap.contains(key) )
    mRefList[ mRefMap.value(key) ].valueSet( val );
  }
