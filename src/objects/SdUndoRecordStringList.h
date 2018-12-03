/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo string list with one int
*/
#ifndef SDUNDORECORDSTRINGLIST_H
#define SDUNDORECORDSTRINGLIST_H

#include "SdUndoRecord.h"
#include <QStringList>

class SdUndoRecordStringList : public SdUndoRecord
  {
    qint32       mVal;      //Value of int
    qint32      *mValSrc;   //Value source for restoring
    QStringList  mList;     //List of string
    QStringList *mListSrc;  //List source for restoring
  public:
    SdUndoRecordStringList( qint32 *val, QStringList *list );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDSTRINGLIST_H
