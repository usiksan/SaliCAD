/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPulsar.h"

SdPulsar* SdPulsar::pulsar;

SdPulsar::SdPulsar(QObject *parent) : QObject(parent)
  {

  }

void SdPulsar::emitCloseProject(SdProject *project)
  {
  emit closeProject( project );
  }

void SdPulsar::emitRenameProject(SdProject *project)
  {
  emit renameProject( project );
  }

void SdPulsar::emitRenameItem(SdProjectItem *item)
  {
  emit renameItem( item );
  }

void SdPulsar::emitInsertItem(SdProjectItem *item)
  {
  emit insertItem( item );
  }

void SdPulsar::emitRemoveItem(SdProjectItem *item)
  {
  emit removeItem( item );
  }

void SdPulsar::emitActivateItem(SdProjectItem *item)
  {
  emit activateItem( item );
  }

