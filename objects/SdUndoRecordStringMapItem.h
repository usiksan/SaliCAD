/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for one string map pair
*/
#ifndef SDUNDORECORDSTRINGMAPITEM_H
#define SDUNDORECORDSTRINGMAPITEM_H

#include "SdUndoRecord.h"
#include "library/SdStringMap.h"

class SdUndoRecordStringMapItem : public SdUndoRecord
  {
    SdStringMap *mStringMap;
    QString      mKey;
    QString      mValue;
    bool         mPresence;   //True if key was present in map
  public:
    SdUndoRecordStringMapItem( SdStringMap *map, const QString key );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
    virtual void redo() override;
  };


#endif // SDUNDORECORDSTRINGMAPITEM_H
