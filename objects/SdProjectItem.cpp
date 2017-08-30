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

#include "SdProjectItem.h"
#include "SdGraphIdent.h"
#include "SdProject.h"
#include "SdPulsar.h"
#include "SdPoint.h"
#include "SdGraph.h"
#include <QSettings>
#include <QDateTime>

#define timeOffsetConstant 1000000000L

SdProjectItem::SdProjectItem() :
  mCreateTime(0),
  mEditEnable(false),
  mAuto(true),
  mTreeItem(nullptr)
  {

  }




QString SdProjectItem::getId() const
  {
  //Id consist from name, user and time creation
  return getType() + mTitle + mAuthor + QString::number(mCreateTime,32);
  }




QString SdProjectItem::getShortId() const
  {
  //Id consist from name and user
  return getType() + mTitle + mAuthor;
  }






QString SdProjectItem::getExtendTitle() const
  {
  return QString("%1 [r%2] (%3)").arg(mTitle).arg( QDateTime::fromSecsSinceEpoch(getTimeFromEpoch()).toString("yy-M-d H:m:s") ).arg(mAuthor);
  }




qint64 SdProjectItem::getTimeFromEpoch() const
  {
  return timeOffsetConstant + static_cast<qint64>(mCreateTime);
  }




void SdProjectItem::setTitle(const QString title)
  {
  //Item author (registered program copy name)
  updateAuthor();
  //Update creation time
  updateCreationTime();
  //Title setup
  mTitle      = title;
  SdPulsar::pulsar->emitRenameItem( this );
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
SdProjectItem *SdProjectItem::setEditEnable(bool edit)
  {
  if( mEditEnable ) {
    if( !edit ) {
      //Disable edit.
      mEditEnable = edit;
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
        getProject()->insertChild( item, getProject()->getUndo() );
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
  int time = static_cast<int>( QDateTime::currentDateTimeUtc().toSecsSinceEpoch() - timeOffsetConstant );
  if( time <= mCreateTime )
    mCreateTime++;
  else
    mCreateTime = time;
  }




void SdProjectItem::updateAuthor()
  {
  QSettings s;
  mAuthor     = s.value( SDK_GLOBAL_ID_MACHINE ).toString();
  }




bool SdProjectItem::isAnotherAuthor() const
  {
  QSettings s;
  return mAuthor != s.value( SDK_GLOBAL_ID_MACHINE ).toString();
  }




//Get over rect
SdRect SdProjectItem::getOverRect(quint64 classMask)
  {
  //Collect fit rect
  SdRect fit;
  forEach( classMask, [&fit](SdObject *obj) {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph )
      fit.grow( graph->getOverRect() );
    return true;
    } );

  return fit;
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




//Upgrade old item to new item
void SdProjectItem::upgradeItem(const SdProjectItem *oldItem, const SdProjectItem *newItem)
  {
  //For each graph objects perform item upgrade
  forEach( dctAll, [oldItem,newItem] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph != nullptr )
      graph->upgradeItem( oldItem, newItem );
    return true;
    });
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
  obj.insert( QStringLiteral("Title"),    mTitle );
  obj.insert( QStringLiteral("Author"),   mAuthor );
  obj.insert( QStringLiteral("Created"),  mCreateTime );
  obj.insert( QStringLiteral("Auto"),     mAuto );
  sdParamWrite( QStringLiteral("Parametrs"), mParamTable, obj );
  mOrigin.write( QStringLiteral("Origin"), obj );
  }




void SdProjectItem::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdContainer::readObject( map, obj );
  mTitle      = obj.value( QStringLiteral("Title") ).toString();
  mAuthor     = obj.value( QStringLiteral("Author") ).toString();
  mCreateTime = obj.value( QStringLiteral("Created") ).toInt();
  mAuto       = obj.value( QStringLiteral("Auto") ).toBool();
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
  mAuto       = true;
  mParamTable = sour->mParamTable;
  mOrigin     = sour->mOrigin;
  }
