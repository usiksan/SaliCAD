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
  qDeleteAll( mChildList );
  }




void SdContainer::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );

  //Read list
  qDeleteAll( mChildList );
  mChildList.clear();
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
    if( ptr ) {
      array.append( ptr->write() );
      }
  obj.insert( QStringLiteral("ChildList"), array );
  }





void SdContainer::forEach(int classMask, std::function<bool (SdObject *)> fun1)
  {
  for( SdObject *ptr : mChildList )
    if( ptr && ptr->getClass() & classMask ) {
      if( !fun1(ptr) ) return;
      }
  }
