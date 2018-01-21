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
#include "SdPoint.h"

class SdGraphSymImp;
class SdGraphSymPin;
class SdGraphPartImp;
class SdGraphPartPin;
class SdGraphRoadPin;
class SdPItemPart;
class SdStratum;

class SdUndoRecordImpPin : public SdUndoRecord
  {
    SdGraphSymImp  *mSymImp;        //Symbol implementation for pin
    QString         mSymPinName;    //Pin index in symbol implementation
    SdGraphSymPin  *mSymPin;        //Symbol pin
    SdPoint         mSymPosition;   //Pin position in sheet context
    SdGraphPartImp *mPartImp;       //Part implementation for pin
    QString         mPartPinNumber; //Pin index in part implementation
    SdGraphPartPin *mPartPin;       //Part pin
    SdPoint         mPartPosition;  //Pin position in plate context
    SdPItemPart    *mPartPadStack;  //Pad stack
    SdStratum       mPartStratum;   //Pin stratum
    SdGraphRoadPin *mPartRoadPin;   //Road pin assotiated with partImp pin

    QString         mWireName;      //Wire name for connection
    bool            mCom;           //Status of connection
  public:
    SdUndoRecordImpPin( SdGraphSymImp *sym, const QString symPinName, SdGraphPartImp *part );
    SdUndoRecordImpPin( SdGraphPartImp *part, const QString partPinNumber );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDIMPPIN_H
