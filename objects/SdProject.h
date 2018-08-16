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
#include "library/SdStringMap.h"
#include <QMap>
#include <QTreeWidgetItem>
#include <QSet>


#define SD_TYPE_PROJECT "Project"

class SdProjectItem;
class SdPItemPlate;
class SdPItemSheet;

class SdProject : public SdContainer
  {
    SdStringMap                mParams;               //Project params
    bool                       mDirty;                //Project dirty flag

    //Not saved
    SdUndo                     mUndo;                 //Undo buffer for project

    int                        mNetIndex;             //Net index for creation default net
  public:
    SdProject();
    ~SdProject() override;

    SdUndo           *getUndo() { return &mUndo; }

    //Param table
    //Get full table
    SdStringMap       paramGetTable() const { return mParams; }
    //Get one param
    QString           paramGet( QString key ) const { return mParams.value(key); }
    //Set one param
    void              paramSet( QString key, QString val );

    //Return first sheet
    SdPItemSheet     *getFirstSheet() const;

    //Return first plate
    SdPItemPlate     *getFirstPlate() const;


    //Return default plate and if none - create new one
    SdPItemPlate     *getDefaultPlate();

    //Return object of project for given object. Return only newer or same object
    // not in editing state
    SdProjectItem    *getFixedProjectItem( SdProjectItem *item );

    //Return net name unused in project
    QString           getUnusedNetName();

    //Accum used layers
    void              accumLayerUsage();

    //Renumeration implements
    void              renumeration();

    //Return dirty status
    bool              isDirty() const { return mDirty; }
    void              setDirty();

    //Return true if object with this name present in project
    bool              isNameUsed( const QString name ) const;
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
    //Test if net name already used
    bool              isNetNameUsed( const QString netName );
  };


typedef SdProject *SdProjectPtr;

extern QSet<SdProjectPtr> sdProjectList;

#endif // SDPROJECT_H
