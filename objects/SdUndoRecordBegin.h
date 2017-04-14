/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDUNDORECORDBEGIN_H
#define SDUNDORECORDBEGIN_H

#include "SdUndoRecord.h"

class SdUndoRecordBegin : public SdUndoRecord
  {
    QString mTitle;
  public:
    SdUndoRecordBegin( const QString tit ) : SdUndoRecord(), mTitle(tit) {}

    // SdUndoRecord interface
  public:
    virtual bool    isStep() const override { return true; }
    virtual QString title() const override { return mTitle; }
  };

#endif // SDUNDORECORDBEGIN_H
