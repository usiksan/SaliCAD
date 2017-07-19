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
#include "SdParamTable.h"
#include "SdPoint.h"
#include "SdSelector.h"
#include <QString>


class SdProject;
class QTreeWidgetItem;
class SdGraphIdent;
class SdWEditorGraph;

class SdProjectItem : public SdContainer
  {
    QString                mTitle;      //Item title
    QString                mAuthor;     //Item author (registered program copy name)
    int                    mCreateTime; //Create time with sec from 2000year

    QString                mReplaceId;  //Newest object id the same name and author
    bool                   mAuto;       //True if item inserted automatic as reference from other item
  protected:
    SdParamTable           mParamTable; //Object parameters
  public:
    SdPoint                mOrigin;     //Origin for object
    QTreeWidgetItem       *mTreeItem;   //Correspond visual tree item

    SdProjectItem();

    //Information
    virtual QString        getId() const override;
    QString                getShortId() const;
    QString                getExtendTitle() const;
    QString                getAuthor() const { return mAuthor; }
    int                    getTime() const { return mCreateTime; }
    qint64                 getTimeFromEpoch() const;
    QString                getTitle() const { return mTitle; }
    void                   setTitle( const QString title );
    SdProject             *getProject() const;
    SdUndo                *getUndo() const;
    void                   setReplaceId( const QString id ) { mReplaceId = id; }
    //Set creation time as current
    void                   updateCreationTime();
    //Set author as current
    void                   updateAuthor();
    //Check if another author
    bool                   isAnotherAuthor() const;
    //Get over rect
    SdRect                 getOverRect( quint64 classMask = dctAll );

    //Object visual (graphical) identificator
    SdGraphIdent          *getIdent();
    virtual SdGraphIdent  *createIdent() = 0;




    void                   setHand() { mAuto = false; }

    virtual QString        getIconName() const = 0;
    virtual quint64        getAcceptedObjectsMask() const = 0;
    virtual SdGraph       *insertCopyObject( const SdGraph *obj, SdPoint offset, SdUndo *undo, SdWEditorGraph *editor, bool next );
    virtual void           insertObjects( SdPoint offset, SdSelector *sour, SdUndo *undo, SdWEditorGraph *editor, SdSelector *dest, bool next );

    virtual void           writeObject(QJsonObject &obj) const override;
    virtual void           readObject(SdObjectMap *map, const QJsonObject obj) override;

    virtual void           cloneFrom( const SdObject *src ) override;
  };

typedef SdProjectItem *SdProjectItemPtr;

#endif // SDPROJECTITEM_H
