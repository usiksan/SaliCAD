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
  mWidth(*width),
  mSrcP1(p1),
  mP1(*p1),
  mSrcP2(p2),
  mP2(*p2)
  {
  }


void SdUndoRecordRoad::undo()
  {
  mSrcWidth->swap( mWidth );
  mP1.swap( mSrcP1 );
  mP2.swap( mSrcP2 );
  }
