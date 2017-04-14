/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo system.
  With every action it store in undo stack by set undo records.
  Every undo step separated with special undo record "title" witch serves as step marker
*/

#ifndef SDUNDO_H
#define SDUNDO_H
#include "SdConfig.h"
#include "SdUndoRecord.h"
#include <QStack>


class SdContainer;
class SdObject;
class SdPoint;
class SdPropInt;
class SdPropLayer;
class SdPointTable;

class SdUndo
  {
    QStack<SdUndoRecordPtr> mUndo;
    QStack<SdUndoRecordPtr> mRedo;
    int                     mUndoCount;
  public:
    SdUndo();

    //info
    bool isUndoPresent() const { return mUndo.count() != 0; }
    bool isRedoPresent() const { return mRedo.count() != 0; }

    //save undo
    void insertObject( SdContainer *container, SdObject *object );
    void deleteObject( SdContainer *container, SdObject *object );
    void point( SdPoint *p );
    void point2( SdPoint *p1, SdPoint *p2 );
    void pointIndex( SdPointTable *table, int index );
    void intValue( int *v );
    void propInt( SdPropInt *propInt );
    void propLayer( SdPropLayer *propLayer );
    void propLayerIntInt( SdPropLayer *propLayer, SdPropInt *propInt1, SdPropInt *propInt2 );
    void propString( QString *str );
    void begin( QString title );

    //do undo and redo
    void undoStep();
    void redoStep();
  private:
    void addUndo( SdUndoRecordPtr ptr );
  };

#endif // SDUNDO_H
