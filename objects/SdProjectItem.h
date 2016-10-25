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
#include <QString>


class SdProject;
class QTreeWidgetItem;

class SdProjectItem : public SdContainer
  {
    QString                mTitle;
    int                    mRefCount;
    bool                   mAuto;
  public:
    QTreeWidgetItem       *mTreeItem;

    SdProjectItem();

    //Information
    virtual quint64        getId();
    QString                getTitle() const { return mTitle; }
    void                   setTitle( const QString title );
    SdProject*             getProject() const;

    void                   addRef() { mRefCount++; }
    void                   decRef();
    int                    refCount() const { return mRefCount; }
    void                   setHand() { mAuto = false; }

    virtual QString        getIconName() const = 0;

    virtual void           writeObject(QJsonObject &obj) const override;
    virtual void           readObject(SdObjectMap *map, const QJsonObject obj) override;

    virtual void           cloneFrom(SdObject *src) override;
  };

typedef SdProjectItem *SdProjectItemPtr;

#endif // SDPROJECTITEM_H
