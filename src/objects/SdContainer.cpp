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
#include "SdJsonIO.h"
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








void SdContainer::forEach(quint64 classMask, std::function<bool (SdObject *)> fun1)
  {
  //for( SdObject *ptr : mChildList )
  for( int i = 0; i < mChildList.count(); i++ ) {
    SdObject *ptr = mChildList[i];
    if( ptr && !ptr->isDeleted() ) {
      //Check if object match class mask
      if( ptr->getClass() & classMask ) {
        if( !fun1(ptr) ) return;
        }
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





//Set param value to local table
void SdContainer::paramSet(const QString key, QString val, SdUndo *undo)
  {
  if( undo != nullptr )
    undo->stringMapItem( &mParamTable, key );
  mParamTable.insert( key, val );
  }







//Delete param from local table
void SdContainer::paramDelete(const QString key, SdUndo *undo)
  {
  if( undo != nullptr )
    undo->stringMapItem( &mParamTable, key );
  mParamTable.remove( key );
  }






//Setup full param table
void SdContainer::paramTableSet(const SdStringMap map, SdUndo *undo)
  {
  if( undo != nullptr )
    undo->prop( &mParamTable ).post( [this]() { setProjectDirtyFlag(); } );
  mParamTable = map;
  setProjectDirtyFlag();
  }






//Return param value from hierarchy. We start from local table
// if param present in local table - return it
// If no param in local table then we look at parent container
// if no parent then return empty string
QString SdContainer::paramHierarchy(const QString key) const
  {
  //We start from local table
  // if param present in local table - return it
  if( mParamTable.contains(key) )
    return mParamTable.value(key);

  // If no param in local table then we look at parent container
  if( getParent() )
    return getParent()->paramHierarchy(key);

  // if no parent then return empty string
  return QString();
  }








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




//!
//! \brief cloneFrom Overrided function. We copy all objects of source container
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdContainer::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdObject::cloneFrom( src, copyMap, next );
  SdPtrConst<SdContainer> sour(src);

  //Copy children
  if( sour.isValid() ) {
    clearChildList();
    for( SdObject *ptr : sour->mChildList )
      if( !ptr->isDeleted() ) {
        //Create copy of object
        SdObject *cp = ptr->copy( copyMap, next );
        if( cp ) {
          //If copy is created, then set parent of copy to this container
          cp->setParent( this );
          //...and append to containers list
          mChildList.append( cp );
          }
        }

    //Copy param table
    mParamTable = sour->mParamTable;
    }
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









//!
//! \brief isChild Test if object obj is child of this container or not
//! \param obj     Tested object
//! \return        true if object is child of this container
//!
bool SdContainer::isChild(const SdObject *obj) const
  {
  for( auto const &ptr : mChildList )
    if( !ptr->isDeleted() && ptr == obj ) return true;
  return false;
  }




void SdContainer::deleteAll(SdUndo *undo)
  {
  if( undo ) {
    forEach( dctAll, [this,undo] (SdObject *obj) -> bool {
      deleteChild( obj, undo );
      return true;
      });
    }
  else clearChildList();
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




//Upgrade project item on new one
bool SdContainer::upgradeProjectItem(SdUndo *undo, QWidget *parent)
  {
  return upgradeClassProjectItem( dctAll, undo, parent );
  }





//Upgrage only class objects with newer items
bool SdContainer::upgradeClassProjectItem(SdClass mask, SdUndo *undo, QWidget *parent )
  {
  //Upgrade all objects of container
  bool res = true;
  forEach( mask, [&res,undo,parent] (SdObject *obj) -> bool {
    return res = obj->upgradeProjectItem( undo, parent );
    });
  return res;
  }





void SdContainer::json(SdJsonWriter &js) const
  {
  auto fun = [] ( const SdObject *ptr ) -> bool { return ptr && !ptr->isDeleted(); };
  js.jsonListPtr( js, QStringLiteral("ChildList"), mChildList, fun );
  js.jsonMapString( QStringLiteral("Parametrs"), mParamTable );
  SdObject::json( js );
  }




void SdContainer::json( const SdJsonReader &js )
  {
  //Assign class for previous version database remapping
  js.property()->mClass = getClass();
  //Postfix function for child objects parent setting
  auto fun = [this] ( SdObject *obj ) -> bool { if( obj != nullptr ) obj->setParent(this); return obj != nullptr; };
  js.jsonListPtr( js, QStringLiteral("ChildList"), mChildList, fun );
  js.jsonMapString( QStringLiteral("Parametrs"), mParamTable );
  SdObject::json( js );
  }
