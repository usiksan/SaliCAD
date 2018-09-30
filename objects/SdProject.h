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
#include "SdProjectSharedPtr.h"

#include <QMap>
#include <QTreeWidgetItem>
#include <QSet>


#define SD_TYPE_PROJECT "Project"

class SdProjectItem;
class SdPItemPlate;
class SdPItemSheet;

class SdProject : public SdContainer
  {
    bool                       mDirty;                //Project dirty flag

    //Not saved
    SdUndo                     mUndo;                 //Undo buffer for project

    int                        mNetIndex;             //Net index for creation default net
  public:
    SdProject();
    ~SdProject() override;

    SdUndo           *getUndo() { return &mUndo; }


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

    //Sheet auto renumeration
    void              sheetRenumeration();

    //Return dirty status
    bool              isDirty() const { return mDirty; }
    void              setDirty();

    //Return true if object with this name present in project
    bool              isNameUsed( const QString name ) const;
    SdObjectPtr       item( QTreeWidgetItem *src ) const;
    SdObjectPtr       itemByName( quint64 mask, const QString name ) const;
    SdObjectPtr       itemByUid( const QString &uid ) const;

    //Return full net list from all project sheets
    QStringList       netList() const;

    //Check newer object and mark for upgrading
    void              newerCheckAndMark();

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
    virtual void      redoDeleteChild( SdObject *child ) override;

  private:
    //Test if net name already used
    bool              isNetNameUsed( const QString netName );
  };


typedef SdProject *SdProjectPtr;

extern QSet<SdProjectPtr> sdProjectList;


#endif // SDPROJECT_H
