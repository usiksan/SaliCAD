/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for one record in pad association table
*/
#include "SdUndoRecordPadAssociation.h"

SdUndoRecordPadAssociation::SdUndoRecordPadAssociation(QString *id, QString *srcName, SdPadMap *srcMap) :
  SdUndoRecord(),
  //Save sources
  mSrcMap(srcMap),
  mSrcName(srcName),
  mSrcId(id)
  {
  //Save information from source
  mMap  = *srcMap;
  mName = *srcName;
  mId   = *id;
  }




void SdUndoRecordPadAssociation::undo()
  {
  mSrcMap->swap( mMap );
  mSrcName->swap( mName );
  mSrcId->swap( mId );
  }

