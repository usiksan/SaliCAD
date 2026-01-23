/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base for all project items
*/
#include "library/SdLibraryStorage.h"
#include "SdProjectItem.h"
#include "SdCopyMapProject.h"
#include "SdGraphIdent.h"
#include "SdGraphValue.h"
#include "SdProject.h"
#include "SdPulsar.h"
#include "SdPoint.h"
#include "SdGraph.h"
#include "SdUtil.h"
#include "Sd3dGraph.h"
#include "SdJsonIO.h"

#include <QDateTime>
#include <QDebug>


SdProjectItem::SdProjectItem() :
  SdContainerFile(),
  mAuto(true),
  mTreeItem(nullptr)
  {

  }






QString SdProjectItem::getToolTip() const
  {
  if( !isEditEnable() ) {
    if( mThereNewer )
      return QObject::tr("There new object. Use upgrade menu. '%1'").arg( getExtendTitle() );
    return QObject::tr("Object locked. Unlock to edit. '%1'").arg( getExtendTitle() );
    }
  return getExtendTitle();
  }





void SdProjectItem::setTitle(const QString title, const QString undoTitle)
  {
  SdUndo *undo = getUndo();
  if( undo != nullptr ) {
    if( !undoTitle.isEmpty() )
      undo->begin( undoTitle, this, false );
    undo->projectItemInfo( this, &mTitle, &mAuthorKey, &mCreateTime, &mEditEnable );
    }

  titleSet( title );
  SdPulsar::sdPulsar->emitRenameItem( this );
  }




void SdProjectItem::setUnicalTitle(const QString undoTitle)
  {
  int test = 1;
  QString title = mTitle;
  do {
    title = mTitle;
    title.append( QString("(%1)").arg(test++) );
    }
  while( SdLibraryStorage::instance()->cfObjectContains( headerUid( getType(), title, getDefaultAuthor() ) ) );
  setTitle( title, undoTitle );
  }







SdProject *SdProjectItem::getProject() const
  {
  return dynamic_cast<SdProject*>( getParent() );
  }




SdUndo *SdProjectItem::getUndo() const
  {
  SdProject *prj = getProject();
  if( prj ) return prj->getUndo();
  return nullptr;
  }






//Set editEnable flag. Return copy object when object editing is prohibited
SdProjectItem *SdProjectItem::setEditEnable( bool edit, const QString undoTitle )
  {
  //mThereNewer = false;
  SdUndo *undo = getUndo();
  if( !undoTitle.isEmpty() && undo )
    undo->begin( undoTitle, this, false );
  if( mEditEnable ) {
    if( !edit ) {
      //Disable edit.
      undo->projectItemInfo( this, &mTitle, &mAuthorKey, &mCreateTime, &mEditEnable );

      mEditEnable = edit;
      //Update object version and author creation
      updateAuthor();
      updateCreationTime();
      //Write object to local library
      SdLibraryStorage::instance()->cfObjectInsert( this );
      //Upgrade item through project
      //getProject()->upgradeProjectItem( this, undo );
      }
    //For each open projects perform check and mark newer objects
    for( auto iter = sdProjectList.cbegin(); iter != sdProjectList.cend(); iter++ )
      (*iter)->newerCheckAndMark();
    }
  else {
    if( edit ) {
      //Enable edit
      //Test if object is used
      if( getProject() && (getProject()->isUsed( this ) || isAnotherAuthor()) ) {
        //Object is used. Create new one
        SdCopyMapProject copyMap( getProject() );
        SdProjectItem *item = dynamic_cast<SdProjectItem*>( copy( copyMap, false ) );
        item->updateAuthor();
        item->updateCreationTime();
        item->mEditEnable = true;
        //Insert item to project
        getProject()->insertChild( item, undo );
        return item;
        }
      mEditEnable = edit;
      updateCreationTime();
      }
    }
  //To update time send signal
  SdPulsar::sdPulsar->emitRenameItem( this );
  return this;
  }






//Find ident
SdGraphIdent *SdProjectItem::findIdent() const
  {
  SdGraphIdent *ident = nullptr;
  forEachConst( dctIdent, [&ident] (SdObject *obj) -> bool {
    ident = dynamic_cast<SdGraphIdent*>(obj);
    return ident == nullptr;
    });
  return ident;
  }




//Find value
SdGraphValue *SdProjectItem::findValue() const
  {
  SdGraphValue *value = nullptr;
  forEachConst( dctValue, [&value] (SdObject *obj) -> bool {
    value = dynamic_cast<SdGraphValue*>(obj);
    return value == nullptr;
    });
  return value;
  }




//Get over rect
SdRect SdProjectItem::getOverRect(quint64 classMask)
  {
  //Collect fit rect
  SdRect fit;
  forEach( classMask, [&fit](SdObject *obj) {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph ) {
      if( fit.isEmpty() )
        fit = graph->getOverRect();
      else
        fit.grow( graph->getOverRect() );
      }
    return true;
    } );

  return fit;
  }








//Object visual (graphical) identificator
SdGraphIdent *SdProjectItem::identGet()
  {
  //Find ident if present
  SdGraphIdent *ident = findIdent();

  //If not found then create default
  if( ident == nullptr )
    return identCreate();
  return ident;
  }




SdGraphIdent *SdProjectItem::identCreate()
  {
  SdGraphIdent *ident;
  if( getClass() & (dctPart | dctPlate) )
    ident = new SdGraphIdent( SdPoint(), SdRect(), sdGlobalProp->mPartIdentProp );
  else
    ident = new SdGraphIdent( SdPoint(), SdRect(), sdGlobalProp->mSymIdentProp );
  SdProject *prj = getProject();
  if( prj == nullptr )
    insertChild( ident, nullptr );
  else
    insertChild( ident, prj->getUndo() );
  return ident;
  }




SdGraphValue *SdProjectItem::valueGet()
  {
  //Find ident if present
  SdGraphValue *value = findValue();

  //If not found then create default
  if( value == nullptr )
    return valueCreate();
  return value;
  }




SdGraphValue *SdProjectItem::valueCreate()
  {
  SdGraphValue *value;
  if( getClass() & (dctPart | dctPlate) )
    value = new SdGraphValue( SdPoint(), SdRect(), sdGlobalProp->mPartValueProp );
  else
    value = new SdGraphValue( SdPoint(), SdRect(), sdGlobalProp->mSymValueProp );
  SdProject *prj = getProject();
  if( prj == nullptr )
    insertChild( value, nullptr );
  else
    insertChild( value, prj->getUndo() );
  return value;
  }





void SdProjectItem::setOrigin(const SdPoint org, SdUndo *undo)
  {
  if( undo ) {
    undo->begin( QObject::tr("Origin position changed"), this, false );
    undo->point( &mOrigin );
    }
  mOrigin = org;
  }





//Delete all item objects
void SdProjectItem::clear()
  {
  deleteAll( getUndo() );
  }




//Set layers usage
void SdProjectItem::setLayerUsage()
  {
  forEach( dctAll, [] (SdObject *obj) ->bool {
    SdPtr<SdGraph> graph(obj);
    if( graph )
      graph->setLayerUsage();
    return true;
    });
  }




//Delete object if it is mAuto=true and no more reference to it
void SdProjectItem::autoDelete(SdUndo *undo)
  {
  if( mAuto && getProject() ) {
    if( !getProject()->isUsed( this ) )
      deleteObject( undo );
    }
  }




//Draw 3d model
void SdProjectItem::draw3d(QOpenGLFunctions_2_0 *f)
  {
  //Draw all 3d objects
  forEach( dct3D, [f] ( SdObject *obj ) ->bool {
    SdPtrConst<Sd3dGraph> obj3d(obj);
    if( obj3d.isValid() )
      obj3d->draw3d( f );
    return true;
    } );
  }






void SdProjectItem::insertObjects(SdPoint offset, SdSelector *sour, SdUndo *undo, SdSelector *dest, bool next)
  {
  SdCopyMapProject copyMap( getProject() );
  sour->forEach( dctAll, [this, dest, undo, offset, next, &copyMap ] (SdGraph *obj) ->bool {
    if( obj != nullptr && (obj->getClass() & getAcceptedObjectsMask()) ) {

      if( obj->getClass() == dctIdent && findIdent() != nullptr )
        //Ident already present in item, preserve insertion
        return true;
      if( obj->getClass() == dctValue && findValue() != nullptr )
        //Value already present in item, presereve isertion
        return true;

      SdGraph *cp = copyMap.copyClass<SdGraph>( obj, next );
      Q_ASSERT( cp != nullptr );
      cp->select( dest );
      cp->move( offset );
      insertChild( cp, undo );
      }
    return true;
    });
  }











//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdProjectItem::json(SdJsonWriter &js) const
  {
  js.jsonBool( QStringLiteral("Auto"),        mAuto );
  js.jsonPoint( QStringLiteral("Origin"),     mOrigin );
  SdContainerFile::json( js );
  }



//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdProjectItem::json( const SdJsonReader &js )
  {
  js.jsonBool( QStringLiteral("Auto"),        mAuto );
  js.jsonPoint( QStringLiteral("Origin"),     mOrigin );
  SdContainerFile::json( js );
  }



//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdProjectItem::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdContainerFile::cloneFrom( src, copyMap, next );
  SdPtrConst<SdProjectItem> sour(src);
  if( sour.isValid() ) {
    mAuto       = true;
    mOrigin     = sour->mOrigin;
    }
  }



