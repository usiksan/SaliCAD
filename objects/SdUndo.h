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
class SdPointList;
class SdPropInt;
class SdPropLayer;
class SdPropLine;
class SdPropText;
class SdPropSymPin;
class SdPropPartPin;
class SdRect;
class SdPItemPlate;
typedef SdPItemPlate *SdPItemPlatePtr;

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
    void propLineAnd2Point( SdPropLine *prp, SdPoint *p1, SdPoint *p2 );
    void propLinePointInt( SdPropLine *prp, SdPoint *p, int *val );
    void propLinePointTable( SdPropLine *prp, SdPointList *list );
    void propTextAndText( SdPropText *prp, SdPoint *org, SdRect *r, QString *str );
    void propSymPin( SdPropSymPin *prp, SdPoint *org );
    void propPartPin( SdPropPartPin *prp, SdPoint *org );
    void platePointer( SdPItemPlatePtr *ptr );
    void point( SdPoint *src );
    void begin( QString title );

    //do undo and redo
    void undoStep();
    void redoStep();
  private:
    void addUndo( SdUndoRecordPtr ptr );
  };

#endif // SDUNDO_H
