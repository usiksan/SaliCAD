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

#include "SdObjectFactory.h"
#include "SdProjectItem.h"
#include "SdGraphIdent.h"
#include "SdProject.h"
#include "SdPulsar.h"
#include "SdPoint.h"
#include "SdGraph.h"
#include "SdUtil.h"
#include <QSettings>
#include <QDateTime>
#include <QDebug>


SdProjectItem::SdProjectItem() :
  mCreateTime(0),
  mAuto(true),
  mEditEnable(true),
  mTreeItem(nullptr)
  {

  }




QString SdProjectItem::getId() const
  {
  //Id consist from name, user and time creation
  return getType() + mTitle + mAuthor + QString::number(mCreateTime,32);
  }








QString SdProjectItem::getExtendTitle() const
  {
  return QString("%1 [r%2] (%3)").arg(mTitle).arg( QDateTime::fromSecsSinceEpoch(getTimeFromEpoch()).toString("yy-M-d H:m:s") ).arg(mAuthor);
  }




qint64 SdProjectItem::getTimeFromEpoch() const
  {
  return SdUtil::time2000toEpoch( mCreateTime );
  }




void SdProjectItem::setTitle(const QString title, const QString undoTitle)
  {
  SdUndo *undo = getUndo();
  if( undo != nullptr ) {
    if( !undoTitle.isEmpty() )
      undo->begin( undoTitle, this );
    undo->projectItemInfo( this, &mTitle, &mAuthor, &mTag, &mCreateTime, &mEditEnable );
    }

  //Item author (registered program copy name)
  updateAuthor();
  //Update creation time
  updateCreationTime();
  //Title setup
  mTitle      = title;
  SdPulsar::pulsar->emitRenameItem( this );
  }




void SdProjectItem::setUnicalTitle(const QString undoTitle)
  {
  QString title = mTitle;
  do {
    title.append( QString("(1)") );
    }
  while( SdObjectFactory::isObjectPresent( title, getDefaultAuthor() ) );
  setTitle( title, undoTitle );
  }




void SdProjectItem::setTag(const QString tag, const QString undoTitle )
  {
  SdUndo *undo = getUndo();
  if( undo != nullptr ) {
    if( !undoTitle.isEmpty() )
      undo->begin( undoTitle, this );
    undo->projectItemInfo( this, &mTitle, &mAuthor, &mTag, &mCreateTime, &mEditEnable );
    }

  //Item author (registered program copy name)
  updateAuthor();
  //Update creation time
  updateCreationTime();
  //Tag setup
  mTag = tag;
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
  hdr.mTag        = mTag;
  hdr.mTime       = getTime();
  hdr.mClass      = getClass();
  hdr.mParamTable = mParamTable;
  }




//Set editEnable flag. Return copy object when object editing is prohibited
SdProjectItem *SdProjectItem::setEditEnable( bool edit, const QString undoTitle )
  {
  SdUndo *undo = getUndo();
  if( !undoTitle.isEmpty() )
    undo->begin( undoTitle, this );
  if( mEditEnable ) {
    if( !edit ) {
      //Disable edit.
      undo->projectItemInfo( this, &mTitle, &mAuthor, &mTag, &mCreateTime, &mEditEnable );

      mEditEnable = edit;
      //Update object version and author creation
      updateAuthor();
      updateCreationTime();
      //Write object to local library
      qDebug() << "disable edit";
      write();
      //Upgrade item throw project
      getProject()->forEach( dctAll, [this,undo] (SdObject *obj) -> bool {
        if( obj != nullptr )
          obj->upgradeProjectItem( this, undo );
        return true;
        });
      }
    }
  else {
    if( edit ) {
      //Enable edit
      //Test if object is used
      if( getProject()->isUsed( this ) || isAnotherAuthor() ) {
        //Object is used. Create new one
        SdProjectItem *item = dynamic_cast<SdProjectItem*>( copy() );
        item->updateAuthor();
        item->updateCreationTime();
        //Insert item to project
        getProject()->insertChild( item, undo );
        return item;
        }
      mEditEnable = edit;
      updateCreationTime();
      }
    }
  return this;
  }




//On call this function time setup after previous time
void SdProjectItem::updateCreationTime()
  {
  int time = SdUtil::getTime2000();
  if( time <= mCreateTime )
    mCreateTime++;
  else
    mCreateTime = time;
  }




void SdProjectItem::updateAuthor()
  {
  mAuthor = getDefaultAuthor();
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





bool SdProjectItem::isCanUpgaded(SdProjectItem *newObj)
  {
  return newObj != nullptr && getClass() == newObj->getClass() && getTitle() == newObj->getTitle();
  }





//Object visual (graphical) identificator
SdGraphIdent *SdProjectItem::getIdent()
  {
  //Find ident if present
  SdGraphIdent *ident = nullptr;
  forEach( dctIdent, [&ident] (SdObject *obj) -> bool {
    ident = dynamic_cast<SdGraphIdent*>(obj);
    return ident == nullptr;
    });

  //If not found then create default
  if( ident == nullptr )
    return createIdent();
  return ident;
  }




SdGraphIdent *SdProjectItem::createIdent()
  {
  SdGraphIdent *ident;
  if( getClass() & (dctPart | dctPlate) )
    ident = new SdGraphIdent( SdPoint(), QStringLiteral("Id"), SdRect(), sdGlobalProp->mPartIdentProp );
  else
   ident = new SdGraphIdent( SdPoint(), QStringLiteral("Id"), SdRect(), sdGlobalProp->mSymIdentProp );
  SdProject *prj = getProject();
  if( prj == nullptr )
    insertChild( ident, nullptr );
  else
    insertChild( ident, prj->getUndo() );
  return ident;
  }





void SdProjectItem::setOrigin(const SdPoint org, SdUndo *undo)
  {
  if( undo ) {
    undo->begin( QObject::tr("Origin position changed"), this );
    undo->point( &mOrigin );
    }
  mOrigin = org;
  }






void SdProjectItem::insertObjects(SdPoint offset, SdSelector *sour, SdUndo *undo, SdWEditorGraph *editor, SdSelector *dest, bool next)
  {
  sour->forEach( dctAll, [this, offset, undo, next, editor, dest ] (SdGraph *obj) ->bool {
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
  sdParamWrite( QStringLiteral("Parametrs"), mParamTable, obj );
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
  sdParamRead( QStringLiteral("Parametrs"), mParamTable, obj );
  mOrigin.read( QStringLiteral("Origin"), obj );
  }




void SdProjectItem::cloneFrom( const SdObject *src )
  {
  SdContainer::cloneFrom( src );
  const SdProjectItem *sour = dynamic_cast<const SdProjectItem*>(src);
  mTitle      = sour->mTitle;
  mAuthor     = sour->mAuthor;
  mCreateTime = sour->mCreateTime;
  mAuto       = false;
  mParamTable = sour->mParamTable;
  mOrigin     = sour->mOrigin;
  mEditEnable = true;
  }




QString SdProjectItem::getDefaultAuthor()
  {
  QSettings s;
  return s.value( SDK_GLOBAL_AUTHOR ).toString();
  }
