/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for text properties and text string.
*/
#ifndef SDUNDORECORDPROPTEXTANDTEXT_H
#define SDUNDORECORDPROPTEXTANDTEXT_H

#include "SdUndoRecord.h"
#include "SdPropText.h"
#include "SdRect.h"

class SdUndoRecordPropTextAndText : public SdUndoRecord
  {
    SdPropText      *mSrcProp;
    SdPropTextState  mPropState;
    SdRect          *mSrcRect;
    SdRect           mRect;
    QString         *mSrcString;
    QString          mString;
  public:
    SdUndoRecordPropTextAndText( SdPropText *prp, SdRect *r, QString *str );

    //Undo elementary operation.
    virtual void    undo() override;
  };

#endif // SDUNDORECORDPROPTEXTANDTEXT_H
