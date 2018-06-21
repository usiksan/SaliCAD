/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for roads
*/
#include "SdUndoRecordRoad.h"

SdUndoRecordRoad::SdUndoRecordRoad(SdPropInt *width, SdPoint *p1, SdPoint *p2) :
  SdUndoRecord(),
  mSrcWidth(width),
  mSrcP1(p1),
  mSrcP2(p2)
  {
  mWidth =  mSrcWidth->getValue();
  mP1    = *mSrcP1;
  mP2    = *mSrcP2;
  }


void SdUndoRecordRoad::undo()
  {
  mWidth = mSrcWidth->swap( mWidth );
  mP1.swap( mSrcP1 );
  mP2.swap( mSrcP2 );
  }
