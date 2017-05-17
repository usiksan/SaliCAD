/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Line Countur - Region.
*/
#include "SdGraphLinearRegion.h"
#include "SdSegment.h"
#include "SdContext.h"
#include "SdSelector.h"

SdGraphLinearRegion::SdGraphLinearRegion() :
  SdGraphLinear(),
  mList(),
  mFlyIndex()
  {

  }




SdGraphLinearRegion::SdGraphLinearRegion(const SdPointList list, const SdPropLine &propLine) :
  SdGraphLinear( propLine ),
  mList( list ),
  mFlyIndex()
  {
  }




void SdGraphLinearRegion::cloneFrom(const SdObject *src)
  {
  SdGraphLinear::cloneFrom( src );
  const SdGraphLinearRegion *region = dynamic_cast<const SdGraphLinearRegion*>(src);
  if( region ) {
    mList = region->mList;
    mFlyIndex.clear();
    }
  }





void SdGraphLinearRegion::writeObject(QJsonObject &obj) const
  {
  SdGraphLinear::writeObject( obj );
  mList.write( QStringLiteral("Vertex"), obj );
  }





void SdGraphLinearRegion::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraphLinear::readObject( map, obj );
  mList.read( QStringLiteral("Vertex"), obj );
  mFlyIndex.clear();
  }




void SdGraphLinearRegion::move(SdPoint offset)
  {
  mList.move( mFlyIndex, offset );
  }





void SdGraphLinearRegion::rotate(SdPoint center, SdAngle angle)
  {
  mList.rotate( center, angle );
  }




void SdGraphLinearRegion::mirror(SdPoint a, SdPoint b)
  {
  mList.mirror( a, b );
  }




void SdGraphLinearRegion::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    //For each pair points of list check segment made by this pair
    //and if test point p is on segment, then append two point of pair to select list
    for( int i = 0; i < mList.count() - 1; ++i )
      if( p.isOnSegment( mList.get(i), mList.get(i+1)) ) {
        if( !getSelector() ) {
          mFlyIndex.clear();
          selector->insert( this );
          }
        if( mList.get(i) == p ) mFlyIndex.insert( i );
        else if( mList.get(i+1) == p ) mFlyIndex.insert( i + 1 );
        else {
          mFlyIndex.insert( i );
          mFlyIndex.insert( i + 1 );
          }
        }
    }
  }





void SdGraphLinearRegion::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    for( int i = 0; i < mList.count() - 1; ++i )
      if( r.isAccross( mList.get(i), mList.get(i+1)) ) {
        if( !getSelector() ) {
          mFlyIndex.clear();
          selector->insert( this );
          }
        mFlyIndex.insert( i );
        mFlyIndex.insert( i + 1 );
        }
    }

  }




void SdGraphLinearRegion::select(SdSelector *selector)
  {
  mFlyIndex.clear();
  for( int i = 0; i < mList.count(); ++i ) mFlyIndex.insert( i );
  selector->insert( this );
  }




SdRect SdGraphLinearRegion::getOverRect() const
  {
  return SdRect( mList.boundingRect() );
  }




void SdGraphLinearRegion::draw(SdContext *dc)
  {
  dc->region( mList, mProp );
  }




bool SdGraphLinearRegion::snapPoint(SdSnapInfo *snap)
  {
  return false;
  }




int SdGraphLinearRegion::behindCursor(SdPoint p)
  {
  if( isVisible() ) {
    for( int i = 0; i < mList.count()-1; ++i )
      if( p.isOnSegment( mList.get(i), mList[i+1]) )
        return getSelector() ? SEL_ELEM : UNSEL_ELEM;
    if( p.isOnSegment( mList.last(), mList.first() ) )
      return getSelector() ? SEL_ELEM : UNSEL_ELEM;
    }
  return 0;
  }




void SdGraphLinearRegion::saveState(SdUndo *undo)
  {
  if( undo )
    undo->propLinePointTable( &mProp, &mList );
  }


