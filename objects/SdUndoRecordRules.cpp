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
#include "SdUndoRecordRules.h"

SdUndoRecordRules::SdUndoRecordRules( SdRuleBlock *pcbSrc, SdRuleBlockMap *mapSrc ) :
  SdUndoRecord(),
  mPcbSrc(pcbSrc),
  mMapSrc(mapSrc)
  {
  //Save current state of sources
  mPcb = *mPcbSrc;
  mMap = *mMapSrc;
  }


void SdUndoRecordRules::undo()
  {
  //Swap pcb rule block
  SdRuleBlock tmp;
  tmp = *mPcbSrc;
  *mPcbSrc = mPcb;
  mPcb = tmp;

  //Swap net rule map
  mMapSrc->swap( mMap );
  }
