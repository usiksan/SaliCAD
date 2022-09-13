#include "Sd3drPointLink.h"

#include <list>


Sd3drPointLink *Sd3drPointLinkList::alloc()
  {
  if( mFreeIndex >= mPoolSize ) {
    mList.push_back( Sd3dPointLinkArray{} );
    mFreeIndex = 0;
    }
  return mList.back()[mFreeIndex++].init();
  }





Sd3drPointLink *Sd3drPointLinkList::addRegion(Sd3drModel *model, const Sd3drFace &face, bool hole)
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

  Sd3drPointLink *region = nullptr;
  //int count = face.count() - 1;
  for( int i = 0; i < face.count(); i++ ) {
    Sd3drPointLink *p = alloc();
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



Sd3drPointLink *Sd3drPointLink::init()
  {
  return mNext = mPrev = this;
  }




Sd3drPointLink *Sd3drPointLink::remove()
  {
  mPrev->mNext = mNext;
  mNext->mPrev = mPrev;
  return mNext;
  }



void Sd3drPointLink::splitRegion(Sd3drPointLink *other, Sd3drPointLinkList *pool)
  {
  Sd3drPointLink *otherStop = pool->alloc();
  Sd3drPointLink *otherStart = pool->alloc();
  (*otherStop) = (*other);
  (*otherStart) = (*this);
  otherStop->mPrev->mNext = otherStop;
  otherStop->mNext = this;
  mPrev = otherStop;

  otherStart->mNext = other;
  otherStart->mPrev->mNext = otherStart;
  other->mPrev = otherStart;
  }




Sd3drPointLink *Sd3drPointLink::appendNext(Sd3drPointLink *other)
  {
  other->mNext = mNext;
  other->mPrev = this;
  mNext->mPrev = other;
  mNext = other;
  return other;
  }



Sd3drPointLink *Sd3drPointLink::appendPrev(Sd3drPointLink *other)
  {
  other->mPrev = mPrev;
  other->mNext = this;
  mPrev->mNext = other;
  mPrev = other;
  return other;
  }



Sd3drPointLink *Sd3drPointLink::lessLeft()
  {
  //Scan region and find most less point
  Sd3drPointLink *less = this;
  for( Sd3drPointLink *ptr = mNext; ptr != this; ptr = ptr->mNext ) {
    if( ptr->isLeft( less ) )
      less = ptr;
    }
  return less;
  }






bool Sd3drPointLink::isTriangle() const
  {
  return mNext->mNext->mNext == this;
  }

Sd3drFace Sd3drPointLink::triangle() const
  {
  Sd3drFace triangle;
  triangle.append( mIndex );
  triangle.append( mNext->mIndex );
  triangle.append( mPrev->mIndex );
  return triangle;
  }
