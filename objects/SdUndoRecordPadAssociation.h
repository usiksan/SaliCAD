/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDUNDORECORDPADASSOCIATION_H
#define SDUNDORECORDPADASSOCIATION_H

#include "SdUndoRecord.h"
#include "SdPadAssociation.h"

class SdUndoRecordPadAssociation : public SdUndoRecord
  {
    SdPadMap  mMap;
    SdPadMap *mSrcMap;
    QString   mName;
    QString  *mSrcName;
    QString   mId;
    QString  *mSrcId;
  public:
    SdUndoRecordPadAssociation( QString *id, QString *srcName, SdPadMap *srcMap );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDPADASSOCIATION_H
