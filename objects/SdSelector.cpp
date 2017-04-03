/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdSelector.h"
#include "SdRect.h"

SdSelector::SdSelector() :
  SdObject(),
  mOwner(false)
  {

  }




SdSelector::~SdSelector()
  {
  if( mOwner ) qDeleteAll(mTable);
  }




void SdSelector::deleteAll()
  {
  for( SdGraph *graph : mTable ) {
    graph->markDeleted(true);
    }
  }




void SdSelector::removeAll()
  {
  for( SdGraph *graph : mTable )
    graph->mSelector = 0;
  mTable.clear();
  }




void SdSelector::remove(SdGraph *graph)
  {
  if( graph->mSelector == this ) {
    graph->mSelector = 0;
    mTable.remove( graph );
    }
  }




void SdSelector::insert(SdGraph *graph)
  {
  //Залезть в объект и проверить текущий селектор
  if( graph->mSelector ) {
    //Объект уже принадлежит какому-то селектору
    if( graph->mSelector != this ) {
      //Это другой селектор, освободить объект от предыдущего селектора
      graph->mSelector->remove( graph );
      }
    else return;
    }
  graph->mSelector = this; //Объект принадлежит данному селектору
  mTable.insert( graph );  //Добавить ссылку на объект в массив
  }




void SdSelector::writeObject(QJsonObject &obj) const
  {
  SdObject::writeObject( obj );
  //TODO writeObject on SdSelector

  }

void SdSelector::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  //TODO readObject on SdSelector
  }



void SdSelector::forEach(quint64 classMask, std::function<bool (SdGraph *)> fun1)
  {
  for( SdGraph *graph : mTable )
    if( graph && !graph->isDeleted() && (graph->getClass() & classMask) ) {
      if( !fun1(graph) ) return;
      }
  }

SdRect SdSelector::getOverRect()
  {
  if( mTable.count() ) {
    SdRect r;
    bool first = true;
    for( SdGraph *graph : mTable )
      if( first ) {
        r = graph->getOverRect();
        first = false;
        }
      else r.grow( graph->getOverRect() );
    return r;
    }
  return SdRect();
  }


