/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for all tracing rules
*/
#ifndef SDUNDORECORDRULES_H
#define SDUNDORECORDRULES_H

#include "SdUndoRecord.h"
#include "SdRuleBlock.h"

class SdUndoRecordRules : public SdUndoRecord
  {
    SdRuleBlock    *mPcbSrc; //Source for pcb rule block
    SdRuleBlock     mPcb;    //Saved state for pcb rule block
    SdRuleBlockMap *mMapSrc; //Source for net rule map
    SdRuleBlockMap  mMap;    //Saved state for net rule map
  public:
    SdUndoRecordRules( SdRuleBlock *pcbSrc, SdRuleBlockMap *mapSrc );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDRULES_H
