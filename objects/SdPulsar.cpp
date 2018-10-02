/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Object to send messages to visual elements
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

void SdPulsar::emitRenameProject(SdProject *project, const QString shortName)
  {
  emit renameProject( project, shortName );
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



//Highlight item in tree view
void SdPulsar::emitHighlightItem(SdProjectItem *item)
  {
  emit highlightItem( item );
  }




void SdPulsar::emitCloseEditView(SdProjectItem *item)
  {
  emit closeEditView( item );
  }



//Activate project wihtout item selection
void SdPulsar::emitActivateProject(SdProject *project, const QString shortName)
  {
  emit activateProject( project, shortName );
  }




//Highlight project without item highlight
void SdPulsar::emitHighlightProject(SdProject *project)
  {
  emit highlightProject( project );
  }



//Browse part in sheet
void SdPulsar::emitBrowseSheetPart(SdProjectItem *sheet, SdProjectItem *plate)
  {
  emit browseSheetPart( sheet, plate );
  }



//Components, selected from sheet
void SdPulsar::emitSelectedParts(SdProjectItem *plate, QStringList list)
  {
  emit selectedParts( plate, list );
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




void SdPulsar::emitHelpTopic(const QString topic)
  {
  emit helpTopic(topic);
  }

