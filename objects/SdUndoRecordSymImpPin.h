/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for symbol pin properties and point.
*/
#ifndef SDUNDORECORDSYMIMPPIN_H
#define SDUNDORECORDSYMIMPPIN_H

#include "SdUndoRecord.h"
#include "SdPropSymPin.h"
#include "SdPoint.h"

class SdGraphSymImp;
class SdGraphPartImp;

class SdUndoRecordSymImpPin : public SdUndoRecord
  {
    SdGraphSymImp  *mSymImp;
    SdGraphPartImp *mPartImp;
    QString         mPinName;   //Pin name in symbol
    QString         mPinNumber; //Pin number in part
    SdPoint         mOrigin;    //Pin point
    QString         mWireName;  //Net, which pin connected to
    bool            mCom;
  public:
    SdUndoRecordSymImpPin();
  };

#endif // SDUNDORECORDSYMIMPPIN_H
