/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for pin connection
*/
#ifndef SDUNDORECORDIMPPINCONNECTION_H
#define SDUNDORECORDIMPPINCONNECTION_H

#include "SdUndoRecord.h"

class SdGraphSymImp;

class SdUndoRecordImpPinConnection : public SdUndoRecord
  {
    SdGraphSymImp *mSymImp;
    QString        mPinName;

  public:
    SdUndoRecordImpPinConnection( );
  };

#endif // SDUNDORECORDIMPPINCONNECTION_H
