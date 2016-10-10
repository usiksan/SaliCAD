/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdContainer.h"
#include <QJsonArray>

SdContainer::SdContainer()
  {

  }




SdContainer::~SdContainer()
  {
  clearChildList();
  }




void SdContainer::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );

  //Read list
  clearChildList();
  QJsonArray array = obj.value( QStringLiteral("ChildList") ).toArray();
  for( int i = 0; i < array.count(); i++ ) {
    SdObject *ptr = SdObject::read( map, array.at(i).toObject() );
    if( ptr )
      mChildList.append( ptr );
    }
  }




void SdContainer::writeObject(QJsonObject &obj) const
  {
  //Write list
  QJsonArray array;
  for( SdObject *ptr : mChildList )
    if( ptr && !ptr->isDeleted() ) {
      array.append( ptr->write() );
      }
  obj.insert( QStringLiteral("ChildList"), array );
  }





void SdContainer::forEach(int classMask, std::function<bool (SdObject *)> fun1)
  {
  for( SdObject *ptr : mChildList )
    if( ptr && !ptr->isDeleted() && ptr->getClass() & classMask ) {
      if( !fun1(ptr) ) return;
      }
  }




void SdContainer::deleteChild(SdObject *child)
  {
  //Test if in this container
  if( child->getParent() == this ) {
    //Detach from hierarhy
    child->detach();
    //Mark as deleted
    child->markDeleted( true );
    }
  }




void SdContainer::insertChild(SdObject *child)
  {
  if( child->getParent() ) {
    //Remove from previous parent
    child->getParent()->remove( child );
    }
  //Insert in list
  mChildList.append( child );
  child->setParent( this );
  //Attach to hierarhy
  child->attach();
  }




void SdContainer::removeChild(SdObject *child)
  {
  //Test if in this container
  if( child->getParent() == this ) {
    //Detach from hierarhy
    child->detach();
    //Remove from list
    mChildList.removeAll( child );
    child->setParent( 0 );
    }
  }


void SdContainer::cloneFrom(SdObject *src)
  {
  SdObject::cloneFrom( src );
  SdContainer *sour = dynamic_cast<SdObject*>(src);
  if( sour ) {
    clearChildList();
    for( SdObject *ptr : sour->mChildList )
      if( !ptr->isDeleted() )
        mChildList.append( ptr->copy() );
    }
  }

void SdContainer::clearChildList()
  {
  qDeleteAll( mChildList );
  mChildList.clear();
  }
