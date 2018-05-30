/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Beginning undo record. It define undo title and foregraund edit object.
*/
#ifndef SDUNDORECORDBEGIN_H
#define SDUNDORECORDBEGIN_H

#include "SdUndoRecord.h"

class SdProjectItem;

class SdUndoRecordBegin : public SdUndoRecord
  {
    QString        mTitle;    //Undo operation title
    SdProjectItem *mEditItem; //Foregraund edit object
  public:
    SdUndoRecordBegin( const QString tit, SdProjectItem *item ) : SdUndoRecord(), mTitle(tit), mEditItem(item) {}

    // SdUndoRecord interface
  public:
    virtual bool    isStep() const override { return true; }
    virtual QString title() const override { return mTitle; }
    virtual void    undo() override;
  };


#endif // SDUNDORECORDBEGIN_H
