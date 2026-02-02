/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Class SdProject - project (file). Unit of design.
*/

#ifndef SDPROJECT_H
#define SDPROJECT_H

#include "SdContainerFile.h"
#include "SdUndo.h"
#include "SdProjectSharedPtr.h"

#include <QMap>
#include <QTreeWidgetItem>
#include <QSet>


#define SD_TYPE_PROJECT "Project"

class SdProjectItem;
class SdPItemPlate;
class SdPItemSheet;

class SdProject : public SdContainerFile
  {
    bool                       mDirty;                //Project dirty flag

    //Not saved
    SdUndo                     mUndo;                 //Undo buffer for project

    int                        mNetIndex;             //Net index for creation default net
  public:
    SdProject();
    ~SdProject() override;

    void              setEditEnable( bool ena ) { mEditEnable = ena; }

    SdUndo           *getUndo() { return &mUndo; }

    //!
    //! \brief getItem   Find project item by name
    //! \param itemClass Item class to find
    //! \param itemName  Item name to find. Used exact name
    //! \return          Found item or nullptr if not found
    //!
    SdProjectItem    *getItem( quint64 itemClass, const QString itemName ) const;

    //!
    //! \brief getSheet  Find sheet by name
    //! \param sheetName Sheet name to find
    //! \return          Sheet ptr or nullptr if not found
    //!
    SdPItemSheet     *getSheet( const QString sheetName ) const;

    //!
    //! \brief getPlate  Find plate by name
    //! \param plateName Plate name to find
    //! \return          Plate ptr or nullptr if not found
    //!
    SdPItemPlate     *getPlate( const QString plateName ) const;

    virtual void      getHeader( SdLibraryHeader &hdr ) const override;


    //Return default plate and if none - create new one
    SdPItemPlate     *getDefaultPlate();

    //Return object of project for given object. Return only newer or same object
    // not in editing state
    SdProjectItem    *getFixedProjectItem(const SdProjectItem *item );

    template <typename SdItemClass>
    SdItemClass      *getFixedProjectItemClass( SdItemClass *item ) { return dynamic_cast<SdItemClass*>( getFixedProjectItem( item ) ); }

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

    //!
    //! \brief isNameUsed Return true if object with this name present in project
    //! \param name       Name for testing
    //! \param mask       Object class mask where will be tested
    //! \return           true if name used for given class
    //!
    bool              isNameUsed(const QString name , SdClass mask) const;

    //!
    //! \brief item Find project item by visual tree widget item
    //! \param src  Tree item object for item
    //! \return     Item whose src is tree object
    //!
    SdObjectPtr       item( QTreeWidgetItem *src ) const;

    //!
    //! \brief item3d Find project item by visual tree widget item for 3d view
    //! \param src    Tree item object for item
    //! \return       Item whose src is tree object
    //!
    SdObjectPtr       item3d( QTreeWidgetItem *src ) const;
    SdObjectPtr       itemByName( quint64 mask, const QString name ) const;
    SdObjectPtr       itemByHashUidName( const QString &theHashUidName ) const;

    //Return full net list from all project sheets
    QStringList       netList() const;

    //Check newer object and mark for upgrading
    void              newerCheckAndMark();

    virtual QString   getType() const override;
    virtual quint64   getClass() const override;


    //!
    //! \brief load  Load project from file
    //! \param fname File name of loaded project
    //! \return      Loaded project
    //!
    static SdProject *load( const QString fname );

    //!
    //! \brief save  Save project to file
    //! \param fname File name for project save to
    //! \return      true if saving successfull
    //!
    bool              save( const QString fname );

    //!
    //! \brief libraryUpdate Update project in library and all its item
    //!
    void              libraryUpdate() const;

    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void      cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    virtual void      insertChild(SdObject *child, SdUndo *undo ) override;
    virtual void      undoInsertChild(SdObject *child) override;
    virtual void      redoInsertChild(SdObject *child) override;
    virtual void      deleteChild(SdObject *child, SdUndo *undo ) override;
    virtual void      undoDeleteChild(SdObject *child) override;
    virtual void      redoDeleteChild( SdObject *child ) override;
    //Upgrade newer objects
    virtual bool      upgradeNewerItems( SdUndo *undo, QWidget *parent );

  private:
    //Test if net name already used
    bool              isNetNameUsed( const QString netName );

  };


typedef SdProject *SdProjectPtr;

extern QSet<SdProjectPtr> sdProjectList;


#endif // SDPROJECT_H
