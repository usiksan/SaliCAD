#ifndef SD3DPOINTLINK_H
#define SD3DPOINTLINK_H

#include "Sd3dModel.h"

#include <QPointF>
#include <list>
#include <array>

class Sd3dPointLinkList;

struct Sd3dPointLink
  {
    QPointF        mPoint;
    int            mIndex;
    Sd3dPointLink *mNext;
    Sd3dPointLink *mPrev;

    Sd3dPointLink *init();

    Sd3dPointLink *remove();

    void           splitRegion( Sd3dPointLink *other, Sd3dPointLinkList *pool );

    void           unionRegion( Sd3dPointLink *other, Sd3dPointLinkList *pool ) { splitRegion( other, pool ); }

    Sd3dPointLink *appendNext( Sd3dPointLink *other );

    Sd3dPointLink *appendPrev( Sd3dPointLink *other );

    Sd3dPointLink *lessLeft();

    bool           isTriangle() const;

    bool           isLeft( const Sd3dPointLink *ptr ) const
      {
      return mPoint.x() < ptr->mPoint.x() || ((mPoint.x() == ptr->mPoint.x()) && (mPoint.y() < ptr->mPoint.y()));
      }

    Sd3drFace      triangle() const;

  };

using Sd3dPointLinkPtr = Sd3dPointLink*;


class Sd3dPointLinkList
  {
    static const int mPoolSize = 1024;
    using Sd3dPointLinkArray = std::array<Sd3dPointLink,mPoolSize>;
    std::list<Sd3dPointLinkArray> mList;
    int mFreeIndex;
  public:
    Sd3dPointLinkList() : mFreeIndex(mPoolSize) {}

    Sd3dPointLink *alloc();

    Sd3dPointLink *addRegion( Sd3dModel *model, const Sd3drFace &face, bool hole );
  };


#endif // SD3DPOINTLINK_H
