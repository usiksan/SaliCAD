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
#include "SdGraph.h"
#include <QJsonArray>
#include <QDebug>
#include <algorithm>

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
    if( ptr ) {
      ptr->setParent( this );
      mChildList.append( ptr );
      }
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





void SdContainer::forEach(quint64 classMask, std::function<bool (SdObject *)> fun1)
  {
  for( SdObject *ptr : mChildList )
    if( ptr && !ptr->isDeleted() ) {
      //Check if object match class mask
      if( ptr->getClass() & classMask ) {
        if( !fun1(ptr) ) return;
        }
      }
  }




void SdContainer::forEachConst(quint64 classMask, std::function<bool (SdObject *)> fun1) const
  {
  for( SdObject *ptr : mChildList )
    if( ptr && !ptr->isDeleted() ) {
      //Check if object match class mask
      if( ptr->getClass() & classMask ) {
        if( !fun1(ptr) ) return;
        }
      }
  }





void SdContainer::sort(std::function<bool(SdObject *, SdObject *)> fun1)
  {
  std::stable_sort( mChildList.begin(), mChildList.end(), fun1 );
  //qStableSort( mChildList.begin(), mChildList.end(), fun1 );
  }




SdObject *SdContainer::behindPoint(quint64 classMask, SdPoint p, int *state)
  {
  SdObject *res = nullptr;
  forEach( classMask, [p,state,&res] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph ) {
      int tmp = graph->behindCursor(p);
      *state |= tmp;
      if( tmp ) res = graph;
      }
    return true;
    });

  return res;
  }




//void SdContainer::forEachDown(quint64 classMask, std::function<bool (SdObject *)> fun1)
//  {
//  for( SdObject *ptr : mChildList )
//    if( ptr && !ptr->isDeleted() ) {
//      //Check if object match class mask
//      if( ptr->getClass() & classMask ) {
//        //Check if it container and if true then down to elements of container
//        if( ptr->isContainer() ) {
//          SdContainer *down = dynamic_cast<SdContainer*>(ptr);
//          if( down )
//            down->forEach( classMask, fun1 );
//          }
//        else if( !fun1(ptr) ) return;
//        }
//      }
//  }




void SdContainer::deleteChild(SdObject *child, SdUndo *undo)
  {
  //Test if in this container
  Q_ASSERT_X( child->getParent() == this, "deleteChild", "delete not own child" );
  //Mark as deleted
  child->markDeleted( true );
  //Detach from hierarhy
  child->detach( undo );
  //Mark as deleted
  child->markDeleted( true );
  if( undo )
    undo->deleteObject( this, child );
  }




void SdContainer::undoDeleteChild(SdObject *child)
  {
  //Test if in this container
  Q_ASSERT_X( child->getParent() == this, "undoDeleteChild", "undo delete not own child" );
  //Mark as not deleted
  child->markDeleted( false );
  //Undo Detach from hierarhy
  child->undoDetach();
  }




void SdContainer::redoDeleteChild(SdObject *child)
  {
  //Test if in this container
  Q_ASSERT_X( child->getParent() == this, "redoDeleteChild", "redo delete not own child" );
  //Mark as deleted
  child->markDeleted( true );
  }




void SdContainer::insertChild( SdObject *child, SdUndo *undo )
  {
  Q_ASSERT_X( child->getParent() == nullptr, "insertChild", "Insertion with parent setuped" );
  //Insert in list
  mChildList.append( child );
  child->setParent( this );
  if( undo != nullptr )
    undo->insertObject( this, child );
  //Attach to hierarhy
  child->attach( undo );
  }




void SdContainer::undoInsertChild(SdObject *child)
  {
  //Test if in this container
  Q_ASSERT_X( child->getParent() == this, "undoInsertChild", "undo insert not own child" );
  //Detach from hierarhy
  child->undoAttach();
  //Mark as deleted
  child->markDeleted(true);
  }




void SdContainer::redoInsertChild(SdObject *child)
  {
  //Test if in this container
  Q_ASSERT_X( child->getParent() == this, "redoInsertChild", "redo insert not own child" );
  child->markDeleted( false );
  }






void SdContainer::cloneFrom( const SdObject *src )
  {
  SdObject::cloneFrom( src );
  const SdContainer *sour = dynamic_cast<const SdContainer*>(src);
  if( sour ) {
    clearChildList();
    for( SdObject *ptr : sour->mChildList )
      if( !ptr->isDeleted() )
        mChildList.append( ptr->copy() );
    }
  }



void SdContainer::draw(SdContext *context)
  {
  forEach( dctAll, [context] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph )
      graph->draw( context );
    return true;
    });
  }





void SdContainer::clearChildList()
  {
  qDeleteAll( mChildList );
  mChildList.clear();
  }




//Return true if used any of objects in container but exclude itself objects in container
bool SdContainer::isUsed(SdObject *test) const
  {
  for( SdObject *ptr : mChildList )
    if( !ptr->isDeleted() ) {
      if( ptr->isUsed( test ) ) return true;
      }
  return false;
  }

