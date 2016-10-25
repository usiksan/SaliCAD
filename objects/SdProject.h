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
#include <QMap>


#define SD_TYPE_PROJECT "Project"

class SdProjectItem;

class SdProject : public SdContainer
  {
    QJsonObject                mProperties; //Project properties
    bool                       mDirty;      //Project dirty flag
    QMap<QString,SdObjectPtr>  mItemMap;
  public:
    SdProject();
    ~SdProject();

    //Return dirty status
    bool              isDirty() const { return mDirty; }

    //Return true if object with this name present in project
    bool              isContains( const QString name ) const { return mItemMap.contains( name ); }
    SdObjectPtr       item( const QString name ) const { return mItemMap.value(name); }

    virtual QString   getType() const override;
    virtual quint64   getClass() const override;

    virtual void      writeObject(QJsonObject &obj) const override;
    virtual void      readObject(SdObjectMap *map, const QJsonObject obj) override;

    static SdProject *load( const QString fname );
    bool              save( const QString fname );

    virtual void      cloneFrom(SdObject *src) override;
    virtual void      insertChild(SdObject *child) override;
    virtual void      undoInsertChild(SdObject *child) override;
    virtual void      deleteChild(SdObject *child) override;
    virtual void      undoDeleteChild(SdObject *child) override;

  private:
    void               fillMap();
  };

#endif // SDPROJECT_H
