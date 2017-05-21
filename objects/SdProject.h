/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPROJECT_H
#define SDPROJECT_H

#include "SdContainer.h"
#include "SdUndo.h"
#include <QMap>
#include <QTreeWidgetItem>


#define SD_TYPE_PROJECT "Project"

class SdProjectItem;
class SdPItemPlate;

class SdProject : public SdContainer
  {
    QJsonObject                mProperties;           //Project properties
    bool                       mDirty;                //Project dirty flag
    QMap<QString,SdObjectPtr>  mItemExtendNameMap;    //Extend name - item assotiation
    SdUndo                     mUndo;
  public:
    SdProject();
    ~SdProject();

    SdUndo           *getUndo() { return &mUndo; }

    //Return default plate and if none - created new
    SdPItemPlate     *getDefaultPlate();

    //Return object of project for given object
    SdProjectItem    *getProjectsItem( SdProjectItem *item );

    //Return object of project for given id
    SdProjectItem    *getProjectsItem( quint64 mask, const QString id );

    //Return dirty status
    bool              isDirty() const { return mDirty; }
    void              setDirty();

    //Return true if object with this name present in project
    bool              isContains( const QString name ) const;
    //SdObjectPtr       itemByName( const QString name ) const { return mItemNameMap.value(name); }
    SdObjectPtr       itemByExtendName( const QString name ) const { return mItemExtendNameMap.value(name); }
    SdObjectPtr       item( QTreeWidgetItem *src ) const;

    virtual QString   getType() const override;
    virtual quint64   getClass() const override;

    virtual void      writeObject(QJsonObject &obj) const override;
    virtual void      readObject(SdObjectMap *map, const QJsonObject obj) override;

    static SdProject *load( const QString fname );
    bool              save( const QString fname );

    virtual void      cloneFrom( const SdObject *src) override;
    virtual void      insertChild(SdObject *child, SdUndo *undo ) override;
    virtual void      undoInsertChild(SdObject *child) override;
    virtual void      redoInsertChild(SdObject *child) override;
    virtual void      deleteChild(SdObject *child, SdUndo *undo ) override;
    virtual void      undoDeleteChild(SdObject *child) override;

  private:
    void               fillMap();
  };

#endif // SDPROJECT_H
