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
    QString        mTitle;    //!< Undo operation title
    SdProjectItem *mEditItem; //!< Foregraund edit object
    bool           mIs3d;     //!< When true, then mEditItem used in 3d view mode
  public:
    SdUndoRecordBegin( const QString tit, SdProjectItem *item, bool is3d ) : SdUndoRecord(), mTitle(tit), mEditItem(item), mIs3d(is3d) {}

    // SdUndoRecord interface
  public:
    virtual bool    isStep() const override { return true; }
    virtual QString title() const override { return mTitle; }
    virtual void    undo() override;
  };


#endif // SDUNDORECORDBEGIN_H
