/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for projectItem info
*/
#ifndef SDUNDORECORDPROJECTITEM_H
#define SDUNDORECORDPROJECTITEM_H

#include "SdUndoRecord.h"

class SdProjectItem;

class SdUndoRecordProjectItem : public SdUndoRecord
  {
    SdProjectItem *mItem;
    QString        mTitle;
    QString       *mTitleSrc;
    QString        mAuthor;
    QString       *mAuthorSrc;
    int            mTimeCreation;
    int           *mTimeCreationSrc;
    bool           mEditEnable;
    bool          *mEditEnableSrc;
  public:
    SdUndoRecordProjectItem( SdProjectItem *item, QString *title, QString *author, int *timeCreation, bool *editEnable );

    //Undo elementary operation.
    virtual void    undo() override;
  };

#endif // SDUNDORECORDPROJECTITEM_H
