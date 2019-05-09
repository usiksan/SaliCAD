#include "SdWEditor3d.h"
#include "SdWView3d.h"

SdWEditor3d::SdWEditor3d(SdProjectItem *item, QWidget *parent) :
  SdWEditor( parent ),
  mItem(item)
  {
  mView = new SdWView3d( item, this );
  setViewport( mView );

  }


SdProjectItem *SdWEditor3d::getProjectItem() const
  {
  return mItem;
  }

