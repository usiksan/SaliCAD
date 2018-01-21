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




SdUndoRecordImpPin::SdUndoRecordImpPin(SdGraphSymImp *sym, const QString symPinName, SdGraphPartImp *part) :
  SdUndoRecord(),
  mSymImp(sym),                  //Symbol implementation for pin
  mSymPinName(symPinName),       //Pin index in symbol implementation
  mSymPin(nullptr),              //Symbol pin
  mPartImp(part),                //Part implementation for pin
  mPartPadStack(nullptr),        //Pad stack
  mPartRoadPin(nullptr),         //Road pin assotiated with partImp pin
  mCom(false)                    //Status of connection
  {
  if( mSymImp != nullptr ) {
    SdSymImpPin *pin = mSymImp->getPin( mSymPinName );
    if( pin == nullptr )
      mSymImp = nullptr;
    else {
      mSymPin        = pin->mPin;
      mSymPosition   = pin->mPosition;
      mWireName      = pin->mWireName;
      mCom           = pin->mCom;
      mPartPinNumber = pin->mPinNumber;
      if( mPartImp != nullptr ) {
        SdPartImpPin *partPin = mPartImp->getPin( mPartPinNumber );
        if( partPin == nullptr )
          mPartImp = nullptr;
        else {
          //Check link
          Q_ASSERT( partPin->mPinName == mSymPinName && partPin->mSection == mSymImp );
          mPartPosition = partPin->mPosition;
          mPartPadStack = partPin->mPadStack;
          mPartStratum  = partPin->mStratum;
          mPartPin      = partPin->mPin;
          }
        }
      }
    }
  }


SdUndoRecordImpPin::SdUndoRecordImpPin(SdGraphPartImp *part, const QString partPinNumber) :
  SdUndoRecord(),
  mSymImp(nullptr),              //Symbol implementation for pin
  mSymPin(nullptr),              //Symbol pin
  mPartImp(part),                //Part implementation for pin
  mPartPinNumber(partPinNumber),
  mPartPadStack(nullptr),        //Pad stack
  mPartRoadPin(nullptr),         //Road pin assotiated with partImp pin
  mCom(false)                    //Status of connection
  {
  if( mPartImp != nullptr ) {
    SdPartImpPin *partPin = mPartImp->getPin( mPartPinNumber );
    if( partPin == nullptr )
      mPartImp = nullptr;
    else {
      //Check link
      Q_ASSERT( partPin->mPinName == mSymPinName && partPin->mSection == mSymImp );
      mPartPosition = partPin->mPosition;
      mPartPadStack = partPin->mPadStack;
      mPartStratum  = partPin->mStratum;
      mPartPin      = partPin->mPin;
      }

    }
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
