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
#include "SdUndoRecordImpPin.h"
#include "SdGraphPartImp.h"
#include "SdGraphSymImp.h"

SdUndoRecordImpPin::SdUndoRecordImpPin(SdGraphSymImp *sym, int symPin, SdGraphPartImp *part, int partPin, const QString wireName, bool com) :
  SdUndoRecord(),
  mSymImp(sym),           //Symbol implementation for pin
  mSymPinIndex(symPin),   //Pin index in symbol implementation
  mPartImp(part),         //Part implementation for pin
  mPartPinIndex(partPin), //Pin index in part implementation
  mPartRoadPin(nullptr),
  mWireName(wireName),    //Wire name for connection
  mCom(com)               //Status of connection
  {
  //if( mPartImp )
  }




void SdUndoRecordImpPin::undo()
  {
  if( mSymImp ) {
    //Temporary store previous state of connection
    QString prevWireName = mSymImp->pinWireName(mSymPinIndex);
    bool    prevCom      = mSymImp->isPinConnected(mSymPinIndex);
    //Undo connection
    mSymImp->symPinConnectionSet( mSymPinIndex, mWireName, mCom );
    if( mPartImp )
      mPartImp->pinConnectionSet( mPartPinIndex, mWireName, mCom );
    //Set connection state prior undo
    mWireName = prevWireName;
    mCom      = prevCom;
    }
  else if( mPartImp ) {
    //Temporary store previous state of connection
    QString prevWireName = mPartImp->pinNetName(mPartPinIndex);
    bool    prevCom      = mPartImp->isPinConnected(mPartPinIndex);
    mPartImp->pinConnectionSet( mPartPinIndex, mWireName, mCom );
    //Set connection state prior undo
    mWireName = prevWireName;
    mCom      = prevCom;
    }
  }
