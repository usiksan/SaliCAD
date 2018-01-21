/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDUNDORECORDPARTIMPPIN_H
#define SDUNDORECORDPARTIMPPIN_H

#include "SdUndoRecord.h"
#include "SdGraphPartImp.h"

class SdUndoRecordPartImpPin : public SdUndoRecord
  {
    SdGraphPartImp *mPartImp;   //Part implementation with pin
    QString         mPinNumber; //Pin number
    SdPartImpPin    mPinState;  //Pin state
  public:
    SdUndoRecordPartImpPin( SdGraphPartImp *partImp, QString pinNumber );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDPARTIMPPIN_H
