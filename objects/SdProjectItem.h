/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPROJECTITEM_H
#define SDPROJECTITEM_H

#include "SdContainer.h"
#include "SdObjectInfo.h"
#include "SdParamTable.h"
#include "SdPoint.h"
#include <QString>


class SdProject;
class QTreeWidgetItem;

class SdProjectItem : public SdContainer
  {
    SdObjectInfo           mObjectInfo; //Object global info searching on

    QString                mReplaceId;  //Newest object id the same name and author
    int                    mRefCount;   //Item ref count to autodelete item from project
    bool                   mAuto;       //True if item inserted automatic as reference from other item
  protected:
    SdParamTable           mParamTable; //Object parameters
  public:
    SdPoint                mOrigin;     //Origin for object
    QTreeWidgetItem       *mTreeItem;   //Correspond visual tree item

    SdProjectItem();

    //Information
    virtual QString        getId() const override;
    QString                getShortId() const { return mObjectInfo.getShortId(); }
    QString                getIdFileName() const;
    QString                getExtendTitle() const;
    QString                getAuthor() const { return mObjectInfo.mAuthor; }
    int                    getTime() const { return mObjectInfo.mCreateTime; }
    QString                getTitle() const { return mObjectInfo.mTitle; }
    void                   setTitle( const QString title );
    SdProject*             getProject() const;
    void                   setReplaceId( const QString id ) { mReplaceId = id; }
    //Set creation time as current
    void                   updateCreationTime() { mObjectInfo.updateCreationTime(); }
    //Set author as current
    void                   updateAuthor() { mObjectInfo.updateAuthor(); }
    //Check if another author
    bool                   isAnotherAuthor() const { return mObjectInfo.isAnotherAuthor(); }


    void                   addRef() { mRefCount++; }
    void                   decRef();
    int                    refCount() const { return mRefCount; }
    void                   setHand() { mAuto = false; }

    virtual QString        getIconName() const = 0;

    virtual void           writeObject(QJsonObject &obj) const override;
    virtual void           readObject(SdObjectMap *map, const QJsonObject obj) override;

    virtual void           cloneFrom( const SdObject *src ) override;
  };

typedef SdProjectItem *SdProjectItemPtr;

#endif // SDPROJECTITEM_H
