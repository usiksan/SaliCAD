#ifndef SD3DRPOINTLINK_H
#define SD3DRPOINTLINK_H

#include "Sd3drModel.h"

#include <QPointF>
#include <list>
#include <array>

class Sd3drPointLinkList;

struct Sd3drPointLink
  {
    QPointF        mPoint;
    int            mIndex;
    Sd3drPointLink *mNext;
    Sd3drPointLink *mPrev;

    Sd3drPointLink *init();

    Sd3drPointLink *remove();

    void           splitRegion( Sd3drPointLink *other, Sd3drPointLinkList *pool );

    void           unionRegion( Sd3drPointLink *other, Sd3drPointLinkList *pool ) { splitRegion( other, pool ); }

    Sd3drPointLink *appendNext( Sd3drPointLink *other );

    Sd3drPointLink *appendPrev( Sd3drPointLink *other );

    Sd3drPointLink *lessLeft();

    bool           isTriangle() const;

    bool           isLeft( const Sd3drPointLink *ptr ) const
      {
      return mPoint.x() < ptr->mPoint.x() || ((mPoint.x() == ptr->mPoint.x()) && (mPoint.y() < ptr->mPoint.y()));
      }

    Sd3drFace      triangle() const;

  };

using Sd3drPointLinkPtr = Sd3drPointLink*;


class Sd3drPointLinkList
  {
    static const int mPoolSize = 1024;
    using Sd3dPointLinkArray = std::array<Sd3drPointLink,mPoolSize>;
    std::list<Sd3dPointLinkArray> mList;
    int mFreeIndex;
  public:
    Sd3drPointLinkList() : mFreeIndex(mPoolSize) {}

    Sd3drPointLink *alloc();

    Sd3drPointLink *addRegion( Sd3drModel *model, const Sd3drFace &face, bool hole );
  };


#endif // SD3DRPOINTLINK_H
