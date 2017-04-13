/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDUNDO_H
#define SDUNDO_H

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
    QStack<SdUndoRecord> mStack;
  public:
    SdUndo();

    //save undo
    void insertObject( SdContainer *container, SdObject *object );
    void deleteObject( SdContainer *container, SdObject *object );
    void point( SdPoint *p );
    void pointIndex( SdPointTable *table, int index );
    void intValue( int *v );
    void propInt( SdPropInt *propInt );
    void propLayer( SdPropLayer *propLayer );
    void propString( QString *str );
    void begin( QString title );

    //do undo
    void undoStep();
    bool isUndoPresent();
  };

#endif // SDUNDO_H
