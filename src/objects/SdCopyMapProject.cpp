#include "SdCopyMapProject.h"
#include "SdProjectItem.h"
#include "SdGraphNet.h"

SdCopyMapProject::SdCopyMapProject(SdProject *prj) :
  mProject(prj)
  {

  }


SdObjectPtr SdCopyMapProject::copy(const SdObjectPtr src, bool next)
  {
  if( mObjectMap.contains(src->getId()) )
    return mObjectMap.value( src->getId() );

  SdPtrConst<SdProjectItem> item(src);
  if( item.isValid() ) {
    //This is project object
    SdObjectPtr obj = mProject->getFixedProjectItem( item.ptr() );
    //Append to map
    mObjectMap.insert( src->getId(), obj );
    return obj;
    }

  //Make copy of object
  SdObjectPtr obj = src->copy( *this, next );

  mObjectMap.insert( src->getId(), obj );

  SdPtr<SdGraphNet> net(obj);
  if( net.isValid() ) {
    //This is net
    //Test if this net with default name
    if( net->getNetName().startsWith(QString(defNetNamePrefix)) ) {
      //This net with default name
      //Test if this net name already in map
      if( !mNetCopyMap.contains(net->getNetName()) )
        //Net name not registered yet - register
        mNetCopyMap.insert( net->getNetName(), mProject->getUnusedNetName() );
      //This name already registered, substitute
      net->setNetName( mNetCopyMap.value(net->getNetName()), nullptr );
      }
    }

  return obj;
  }
