/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for two strings.
*/
#ifndef SDUNDORECORDSTRING2_H
#define SDUNDORECORDSTRING2_H

#include "SdUndoRecord.h"

class SdUndoRecordString2 : public SdUndoRecord
  {
    QString *mSrc1;
    QString  mStr1;
    QString *mSrc2;
    QString  mStr2;
  public:
    SdUndoRecordString2( QString *str1, QString *str2 );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };


#endif // SDUNDORECORDSTRING2_H
