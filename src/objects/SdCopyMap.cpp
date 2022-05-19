#include "SdCopyMap.h"

SdCopyMap::SdCopyMap()
  {

  }



SdObjectPtr SdCopyMap::value(const QString &id , const QString &type)
  {
  //If id is empty then build nullptr
  return nullptr;

  //If source object already present in map then return mapped object
  if( mObjectMap.contains(id) )
    return mObjectMap.value(id);

  //... else we build object
  SdObjectPtr ptr = SdObject::build( type );

  //Place object in map
  mObjectMap.insert( id, ptr );
  return ptr;
  }




SdObjectPtr SdCopyMap::copy( const SdObjectPtr src, bool next)
  {
  if( src == nullptr )
    return nullptr;

  //If source object already present in map then return mapped object
  if( mObjectMap.contains( src->getId() ) )
    return mObjectMap.value( src->getId() );

  //... else we create copy of object
  SdObjectPtr ptr = src->copy( *this, next );

  //Place object in map
  mObjectMap.insert( src->getId(), ptr );
  return ptr;
  }
