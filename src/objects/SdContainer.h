/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdContainer - container object contains other objects with param table

  Param table is map of key-value pairs. Value is a string.
*/

#ifndef SDCONTAINER_H
#define SDCONTAINER_H

#include "SdObject.h"
#include "SdPoint.h"
#include "library/SdStringMap.h"

#include <QList>
#include <QMap>
#include <functional>

class SdContext;

using SdObjectPtrList = QList<SdObjectPtr>;

class SdContainer : public SdObject
  {
  protected:
    SdObjectPtrList  mChildList;  //!< List of child objects
    SdStringMap      mParamTable; //!< Object parameters
  public:
    SdContainer();
    ~SdContainer() override;

    //Information
    virtual bool isContainer() const override { return true; }

    virtual void insertChild( SdObject *child, SdUndo *undo );
    virtual void undoInsertChild( SdObject *child );
    virtual void redoInsertChild( SdObject *child );

    virtual void deleteChild( SdObject *child, SdUndo *undo );
    virtual void undoDeleteChild( SdObject *child );
    virtual void redoDeleteChild( SdObject *child );


    //!
    //! \brief cloneFrom Overrided function. We copy all objects of source container
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief isChild Test if object obj is child of this container or not
    //! \param obj     Tested object
    //! \return        true if object is child of this container
    //!
    bool         isChild( const SdObject *obj ) const;

            void deleteAll( SdUndo *undo );

            void draw( SdContext *context );
/*[] (SdObject *obj) -> bool {

   }
   */
            void forEach( quint64 classMask, std::function<bool(SdObject*)> fun1 );
            void forEachConst( quint64 classMask, std::function<bool(SdObject*)> fun1 ) const;

            void sort( std::function<bool(SdObject *obj1, SdObject *obj2)> fun1 );

    //Return object and its state behind point
    SdObject    *behindPoint( quint64 classMask, SdPoint p, int *state );



    //Params with local param table
    //Test if param present in local table
    bool         paramContains( const QString key ) const { return mParamTable.contains(key); }

    //Get param value from local table
    QString      paramGet( const QString key ) const { return mParamTable.value(key); }

    //Set param value to local table
    void         paramSet( const QString key, QString val, SdUndo *undo );

    //Delete param from local table
    void         paramDelete( const QString key, SdUndo *undo );

    //Full local param table
    SdStringMap  paramTable() const { return mParamTable; }

    //Setup full param table
    void         paramTableSet( const SdStringMap map, SdUndo *undo );

    //Return param value from hierarchy. We start from local table
    // if param present in local table - return it
    // If no param in local table then we look at parent container
    // if no parent then return empty string
    QString      paramHierarchy( const QString key ) const;


  private:
    void         clearChildList();

    // SdObject interface
  public:
    //Return true if used any of objects in container but exclude itself objects in container
    virtual bool isUsed(SdObject *test) const override;

    //Upgrade project item on new one
    virtual bool upgradeProjectItem( SdUndo *undo, QWidget *parent ) override;

    //Upgrage only class objects with newer items
    bool         upgradeClassProjectItem(SdClass mask, SdUndo *undo , QWidget *parent);

    // SdObject interface
  public:
    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void json( const SdJsonReader &js ) override;
  };


#endif // SDCONTAINER_H
