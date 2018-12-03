/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo link beatween symbol implementation and part implementation
*/
#ifndef SDUNDORECORDLINKSECTION_H
#define SDUNDORECORDLINKSECTION_H

#include "SdUndoRecord.h"

class SdGraphSymImp;
class SdGraphPartImp;

class SdUndoRecordLinkSection : public SdUndoRecord
  {
    int             mSection;
    bool            mLink;
    SdGraphSymImp  *mSym;
    SdGraphPartImp *mPart;
  public:
    SdUndoRecordLinkSection(int section, SdGraphSymImp *sym, SdGraphPartImp *part , bool link);

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDLINKSECTION_H
