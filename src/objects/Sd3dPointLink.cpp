#include "Sd3dPointLink.h"

#include <list>


Sd3dPointLink *Sd3dPointLinkList::alloc()
  {
  if( mFreeIndex >= mPoolSize ) {
    mList.push_back( Sd3dPointLinkArray{} );
    mFreeIndex = 0;
    }
  return mList.back()[mFreeIndex++].init();
  }





Sd3dPointLink *Sd3dPointLinkList::addRegion(Sd3dModel *model, const Sd3drFace &face, bool hole)
  {
  if( face.count() == 0 )
    return nullptr;

  //Detect region direction
  double dir = 0;
  QPointF prev = model->point( face.at(face.count() - 1) );
  for( int i = 0; i < face.count(); i++ ) {
    auto p = model->point( face.at(i) );
    dir += (p.x() - prev.x()) * (p.y() + prev.y());
    prev = p;
    }
  //If wrong direction we revert it
  if( dir >= 0 )
    hole = !hole;

  Sd3dPointLink *region = nullptr;
  //int count = face.count() - 1;
  for( int i = 0; i < face.count(); i++ ) {
    Sd3dPointLink *p = alloc();
    p->mIndex = face.at(i);
    p->mPoint = model->point( p->mIndex );
    if( region == nullptr )
      region = p;
    else if( hole )
      region = region->appendPrev( p );
    else
      region = region->appendNext( p );
    }
  return region;
  }



Sd3dPointLink *Sd3dPointLink::init()
  {
  return mNext = mPrev = this;
  }




Sd3dPointLink *Sd3dPointLink::remove()
  {
  mPrev->mNext = mNext;
  mNext->mPrev = mPrev;
  return mNext;
  }



void Sd3dPointLink::splitRegion(Sd3dPointLink *other, Sd3dPointLinkList *pool)
  {
  Sd3dPointLink *otherStop = pool->alloc();
  Sd3dPointLink *otherStart = pool->alloc();
  (*otherStop) = (*other);
  (*otherStart) = (*this);
  otherStop->mPrev->mNext = otherStop;
  otherStop->mNext = this;
  mPrev = otherStop;

  otherStart->mNext = other;
  otherStart->mPrev->mNext = otherStart;
  other->mPrev = otherStart;
  }




Sd3dPointLink *Sd3dPointLink::appendNext(Sd3dPointLink *other)
  {
  other->mNext = mNext;
  other->mPrev = this;
  mNext->mPrev = other;
  mNext = other;
  return other;
  }



Sd3dPointLink *Sd3dPointLink::appendPrev(Sd3dPointLink *other)
  {
  other->mPrev = mPrev;
  other->mNext = this;
  mPrev->mNext = other;
  mPrev = other;
  return other;
  }



Sd3dPointLink *Sd3dPointLink::lessLeft()
  {
  //Scan region and find most less point
  Sd3dPointLink *less = this;
  for( Sd3dPointLink *ptr = mNext; ptr != this; ptr = ptr->mNext ) {
    if( ptr->isLeft( less ) )
      less = ptr;
    }
  return less;
  }






bool Sd3dPointLink::isTriangle() const
  {
  return mNext->mNext->mNext == this;
  }

Sd3drFace Sd3dPointLink::triangle() const
  {
  Sd3drFace triangle;
  triangle.append( mIndex );
  triangle.append( mNext->mIndex );
  triangle.append( mPrev->mIndex );
  return triangle;
  }
