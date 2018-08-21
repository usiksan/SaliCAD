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
#include <QList>
#include <QMap>
#include <functional>

class SdContext;

typedef QList<SdObjectPtr> SdObjectPtrList;

class SdContainer : public SdObject
  {
  protected:
    SdObjectPtrList  mChildList;  //List of child objects
    SdStringMap      mParamTable; //Object parameters
  public:
    SdContainer();
    ~SdContainer() override;

    //Information
    virtual bool isContainer() const override { return true; }

    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;

    virtual void writeObject(QJsonObject &obj) const override;

    virtual void insertChild( SdObject *child, SdUndo *undo );
    virtual void undoInsertChild( SdObject *child );
    virtual void redoInsertChild( SdObject *child );

    virtual void deleteChild( SdObject *child, SdUndo *undo );
    virtual void undoDeleteChild( SdObject *child );
    virtual void redoDeleteChild( SdObject *child );


    virtual void cloneFrom( const SdObject *src) override;

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
  };


#endif // SDCONTAINER_H
