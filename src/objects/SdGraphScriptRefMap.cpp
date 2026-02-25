#include "SdGraphScriptRefMap.h"
#include "SdPropLine.h"

SdGraphScriptRefMap::SdGraphScriptRefMap()
  {

  }

void SdGraphScriptRefMap::clear()
  {

  }



void SdGraphScriptRefMap::setText(int index, QString sour)
  {
  if( index >= 0 && index < mRefList.count() )
    mRefList[index].valueSet( sour );
  }



int SdGraphScriptRefMap::tryLink(SdPoint p, SdRect &overRect) const
  {
  for( int i = 0; i < mRefList.count(); i++ )
    if( mRefList.at(i).overName().isPointInside(p) ) {
      overRect = mRefList.at(i).overName();
      return i;
      }
  return -1;
  }



void SdGraphScriptRefMap::link(int index, SdContainer *owner, SdGraphParam *ref, const QString paramName)
  {
  if( index >= 0 && index < mRefList.count() )
    mRefList[index].refAssign( owner, ref, paramName );
  }




void SdGraphScriptRefMap::drawExcept(SdPoint p, const SdPropText &prop, SdContext *dc, int exceptIndex)
  {
  //Draw ref map as table like "refName = value"
  mOverRect = SdRect( p, p );
  for( int i = 0; i < mRefList.count(); i++ ) {
    mRefList[i].draw( p, prop, dc, i != exceptIndex );
    p.ry() -= mRefList.at(i).overName().height() + 5;
    mOverRect.grow( mRefList.at(i).overName() );
    mOverRect.grow( mRefList.at(i).overValue() );
    }
  }



//!
//! \brief drawLinks Draw lines connected each variable with its referenced param
//! \param dc        Draw context
//!
void SdGraphScriptRefMap::drawLinks(SdPoint org, SdContext *dc)
  {
  for( int i = 0; i < mRefList.count(); i++ )
    mRefList[i].drawLink( org, dc );
  }


int SdGraphScriptRefMap::behindText(SdPoint p, SdPoint &org, QString &dest)
  {
  for( int i = 0; i < mRefList.count(); i++ )
    if( mRefList.at(i).overValue().isPointInside(p) ) {
      org = mRefList.at(i).originValue();
      dest = mRefList.at(i).valueGet();
      return i;
      }
  return -1;
  }




void SdGraphScriptRefMap::cloneLinks(QPoint org, const SdGraphScriptRefMap &refMap)
  {
  for( int i = 0; i < mRefList.count(); i++ ) {
    mRefList[i].mRefOffset = refMap.linkOffset( org, mRefList.at(i).name() );
    mRefList[i].valueSet( refMap.varGet(mRefList.at(i).name()) );
    mRefList[i].refAssign( nullptr, nullptr, refMap.linkParam(mRefList.at(i).name()) );
    }
  }




void SdGraphScriptRefMap::updateLinks( QPoint org, SdContainer *sheet )
  {
  if( sheet == nullptr ) {
    //Reset all links
    for( int i = 0; i < mRefList.count(); i++ )
      mRefList[i].refAssign( nullptr, nullptr, QString{} );
    }
  else {
    //Update all links
    for( int i = 0; i < mRefList.count(); i++ ) {
      QPoint offset = mRefList.at(i).mRefOffset;
      if( !offset.isNull() ) {
        offset += org;
        int state;
        SdPtr<SdGraphParam> obj( sheet->behindPoint( dctGraphScript | dctSymImp, offset, &state ) );
        if( obj.isValid() )
          mRefList[i].refAssign( sheet, obj.ptr(), QString{} );
        else
          mRefList[i].refAssign( nullptr, nullptr, QString{} );
        }
      }
    }
  }




//!
//! \brief fillLinks Fills mRefOffset fields in SdGraphScriptRef
//!
void SdGraphScriptRefMap::fillLinks(SdPoint org )
  {
  for( int i = 0; i < mRefList.count(); i++ )
    mRefList[i].mRefOffset = mRefList.at(i).refOffset( org );
  }




//!
//! \brief clearCalculations Reset mCalculated field in SdGraphScriptRef
//!                          so that it is correctly set during the calculation
//!
void SdGraphScriptRefMap::clearCalculated()
  {
  for( int i = 0; i < mRefList.count(); i++ )
    mRefList[i].clearCalculated();
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



void SdGraphScriptRefMap::json(SdJsonWriter &js) const
  {
  js.jsonList( js, "list", mRefList );
  }

void SdGraphScriptRefMap::json(SdJsonReader &js)
  {
  js.jsonList( js, "list", mRefList );

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




QPoint SdGraphScriptRefMap::linkOffset( QPoint org, const QString &key ) const
  {
  if( mRefMap.contains(key) )
    return mRefList.at( mRefMap.value(key) ).refOffset( org );
  return QPoint{};
  }



QString SdGraphScriptRefMap::linkParam(const QString &key) const
  {
  if( mRefMap.contains(key) )
    return mRefList.at( mRefMap.value(key) ).refParam();
  return QString{};
  }
