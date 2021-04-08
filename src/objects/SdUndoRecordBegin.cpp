/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Beginning undo record. It define undo title and foregraund edit object.
*/

#include "SdUndoRecordBegin.h"
#include "SdPulsar.h"

void SdUndoRecordBegin::undo()
  {
  //Bring to top edit object
  if( mIs3d )
    SdPulsar::sdPulsar->emitActivateItem3d( mEditItem );
  else
    SdPulsar::sdPulsar->emitActivateItem( mEditItem );
  }
