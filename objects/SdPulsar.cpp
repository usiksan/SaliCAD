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

SdPulsar* SdPulsar::sdPulsar;

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

void SdPulsar::emitCloseEditView(SdProjectItem *item)
  {
  emit closeEditView( item );
  }





void SdPulsar::emitSetStatusLabels(const QString xlabel, const QString ylabel)
  {
  emit setStatusLabels( xlabel, ylabel );
  }

void SdPulsar::emitSetStatusPositions(const QString x, const QString y)
  {
  emit setStatusPositions( x, y );
  }

void SdPulsar::emitSetStatusMessage(const QString msg)
  {
  emit setStatusMessage(msg);
  }




//Emit signal on viewed layers changed. It emited when layer dialog completed
void SdPulsar::emitViewedLayers()
  {
  emit viewedLayers();
  }

