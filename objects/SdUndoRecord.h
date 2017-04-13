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

//struct SdUndoRecord {
//    int mUndoType;
//    union {
//        SdContainer *mContainer;
//      };
//  };

class SdUndoRecord
  {
  public:
    SdUndoRecord() {}

    //Undo elementary operation. Return true on last undo (first operation in undo block)
    virtual bool undo() { return true; }
  };

#endif // SDUNDORECORD_H
