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




//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphLinearRegion::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraphLinear::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphLinearRegion> region(src);
  if( region.isValid() ) {
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





void SdGraphLinearRegion::rotate(SdPoint center, SdPropAngle angle)
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
  if( selector != nullptr )
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




//Find snap point on object
void SdGraphLinearRegion::snapPoint(SdSnapInfo *snap)
  {
  if( snap->match(snapEndPoint) ) {
    //Test for all points of region
    for( SdPoint p : mList )
      snap->test( this, p, snapEndPoint );
    }
  }




int SdGraphLinearRegion::behindCursor(SdPoint p)
  {
  if( isVisible() ) {
    for( int i = 0; i < mList.count()-1; ++i )
      if( p.isOnSegment( mList.get(i), mList.get(i+1)) )
        return getSelector() ? ELEM_SEL : ELEM_UNSEL;
    if( p.isOnSegment( mList.last(), mList.first() ) )
      return getSelector() ? ELEM_SEL : ELEM_UNSEL;
    }
  return 0;
  }




void SdGraphLinearRegion::saveState(SdUndo *undo)
  {
  if( undo )
    undo->propLinePointTable( &mProp, &mList );
  }


