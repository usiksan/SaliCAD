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
#include "library/SdLibraryHeader.h"
#include "SdObjectFactory.h"
#include "SdProjectItem.h"
#include "SdGraphIdent.h"
#include "SdGraphValue.h"
#include "SdProject.h"
#include "SdPulsar.h"
#include "SdPoint.h"
#include "SdGraph.h"
#include "SdUtil.h"
#include "SdTime2x.h"
#include <QSettings>
#include <QDateTime>
#include <QDebug>


SdProjectItem::SdProjectItem() :
  mCreateTime(0),
  mAuto(true),
  mEditEnable(true),
  mTreeItem(nullptr),
  mThereNewer(false)
  {

  }




QString SdProjectItem::getUid() const
  {
  //Id consist from name, user and time creation
  return headerUid( getType(), mTitle, mAuthor );
  }








QString SdProjectItem::getExtendTitle() const
  {
  return QString("%1 [r%2] (%3)").arg(mTitle).arg( SdTime2x::toLocalString(getTime()) ).arg(mAuthor);
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
      undo->begin( undoTitle, this );
    undo->projectItemInfo( this, &mTitle, &mAuthor, &mCreateTime, &mEditEnable );
    }

  //Item author (registered program copy name)
  updateAuthor();
  //Update creation time
  updateCreationTime();
  //Title setup
  mTitle      = title;
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
  while( SdObjectFactory::isObjectPresent( headerUid( getType(), title, getDefaultAuthor() ) ) );
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




void SdProjectItem::getHeader(SdLibraryHeader &hdr) const
  {
  hdr.mName       = mTitle;
  hdr.mType       = getType();
  hdr.mAuthor     = mAuthor;
  hdr.mTime       = getTime();
  hdr.mClass      = getClass();
  hdr.mParamTable = mParamTable;
  }




//Set editEnable flag. Return copy object when object editing is prohibited
SdProjectItem *SdProjectItem::setEditEnable( bool edit, const QString undoTitle )
  {
  //mThereNewer = false;
  SdUndo *undo = getUndo();
  if( !undoTitle.isEmpty() && undo )
    undo->begin( undoTitle, this );
  if( mEditEnable ) {
    if( !edit ) {
      //Disable edit.
      undo->projectItemInfo( this, &mTitle, &mAuthor, &mCreateTime, &mEditEnable );

      mEditEnable = edit;
      //Update object version and author creation
      updateAuthor();
      updateCreationTime();
      //Write object to local library
      write();
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
        SdProjectItem *item = dynamic_cast<SdProjectItem*>( copy() );
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




//On call this function time setup after previous time
void SdProjectItem::updateCreationTime()
  {
  int time = SdTime2x::current();
  if( time <= mCreateTime )
    mCreateTime++;
  else
    mCreateTime = time;
  }




void SdProjectItem::updateAuthor()
  {
  mAuthor = getDefaultAuthor();
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




bool SdProjectItem::isAnotherAuthor() const
  {
  return mAuthor != getDefaultAuthor();
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
    undo->begin( QObject::tr("Origin position changed"), this );
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






void SdProjectItem::insertObjects(SdPoint offset, SdSelector *sour, SdUndo *undo, SdWEditorGraph *editor, SdSelector *dest, bool next)
  {
  sour->forEach( dctAll, [this, editor, dest, undo, offset, next ] (SdGraph *obj) ->bool {
    obj = insertCopyObject( obj, offset, undo, editor, next );
    if( obj != nullptr && dest != nullptr )
      obj->select( dest );
    return true;
    });
  }




SdGraph *SdProjectItem::insertCopyObject(const SdGraph *obj, SdPoint offset, SdUndo *undo, SdWEditorGraph *editor, bool next)
  {
  Q_UNUSED(editor)
  if( obj != nullptr && (obj->getClass() & getAcceptedObjectsMask()) ) {

    if( obj->getClass() == dctIdent && findIdent() != nullptr )
      //Ident already present in item, preserve insertion
      return nullptr;
    if( obj->getClass() == dctValue && findValue() != nullptr )
      //Value already present in item, presereve isertion
      return nullptr;

    SdGraph *cp = dynamic_cast<SdGraph*>( next ? obj->copyNext() : obj->copy() );
    Q_ASSERT( cp != nullptr );
    cp->select( nullptr );
    cp->move( offset );
    insertChild( cp, undo );
    return cp;
    }
  return nullptr;
  }





void SdProjectItem::writeObject(QJsonObject &obj) const
  {
  SdContainer::writeObject( obj );
  obj.insert( QStringLiteral("Title"),       mTitle );
  obj.insert( QStringLiteral("Author"),      mAuthor );
  obj.insert( QStringLiteral("Created"),     mCreateTime );
  obj.insert( QStringLiteral("Auto"),        mAuto );
  obj.insert( QStringLiteral("Edit enable"), mEditEnable );
  mOrigin.write( QStringLiteral("Origin"), obj );
  }




void SdProjectItem::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdContainer::readObject( map, obj );
  mTitle        = obj.value( QStringLiteral("Title") ).toString();
  mAuthor       = obj.value( QStringLiteral("Author") ).toString();
  mCreateTime   = obj.value( QStringLiteral("Created") ).toInt();
  mAuto         = obj.value( QStringLiteral("Auto") ).toBool();
  mEditEnable   = obj.value( QStringLiteral("Edit enable") ).toBool();
  mOrigin.read( QStringLiteral("Origin"), obj );
  }




void SdProjectItem::cloneFrom( const SdObject *src )
  {
  SdContainer::cloneFrom( src );
  const SdProjectItem *sour = dynamic_cast<const SdProjectItem*>(src);
  mTitle      = sour->mTitle;
  mAuthor     = sour->mAuthor;
  mCreateTime = sour->mCreateTime;
  mAuto       = true;
  mOrigin     = sour->mOrigin;
  mEditEnable = sour->mEditEnable;
  }



//Return current registered author
QString SdProjectItem::getDefaultAuthor()
  {
  QSettings s;
  return s.value( SDK_GLOBAL_AUTHOR ).toString();
  }
