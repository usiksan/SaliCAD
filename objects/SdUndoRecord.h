/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base undo record.
  Undo system consist from list of SdUndoRecord. When user do anything SdUndoRecord-s pushed
  in undo stack. When need undo - record poped from stack and executed.
*/

#ifndef SDUNDORECORD_H
#define SDUNDORECORD_H

#include <QString>

class SdUndoRecord
  {
  public:
    SdUndoRecord() {}
    virtual ~SdUndoRecord() {}


    //Undo elementary operation.
    virtual void    undo() { }

    //Redo elementary operation.
    virtual void    redo() { undo(); }

    //Return true on first redo or undo (first operation in undo block)
    virtual bool    isStep() const { return false; }

    virtual QString title() const { return QString(); }
  };

typedef SdUndoRecord *SdUndoRecordPtr;
#endif // SDUNDORECORD_H
