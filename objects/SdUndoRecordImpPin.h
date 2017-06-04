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
#ifndef SDUNDORECORDIMPPIN_H
#define SDUNDORECORDIMPPIN_H

#include "SdUndoRecord.h"

class SdGraphSymImp;
class SdGraphPartImp;
class SdGraphRoadPin;

class SdUndoRecordImpPin : public SdUndoRecord
  {
    SdGraphSymImp  *mSymImp;        //Symbol implementation for pin
    int             mSymPinIndex;   //Pin index in symbol implementation
    SdGraphPartImp *mPartImp;       //Part implementation for pin
    int             mPartPinIndex;  //Pin index in part implementation
    SdGraphRoadPin *mPartRoadPin;   //Road pin assotiated with partImp pin
    QString         mWireName;      //Wire name for connection
    bool            mCom;           //Status of connection
  public:
    SdUndoRecordImpPin( SdGraphSymImp *sym, int symPin, SdGraphPartImp *part, int partPin, const QString wireName, bool com );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDIMPPIN_H
