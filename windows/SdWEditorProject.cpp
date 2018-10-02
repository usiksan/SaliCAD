/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Project view in editors tab workspace
*/
#include "SdWEditorProject.h"
#include "objects/SdPulsar.h"

SdWEditorProject::SdWEditorProject(SdProject *prj, const QString shortName) :
  SdWEditor(),
  mProject(prj),
  mShortName(shortName)
  {
  connect( SdPulsar::sdPulsar, &SdPulsar::renameProject, this, &SdWEditorProject::onProjectRename );
  }


SdProjectItem *SdWEditorProject::getProjectItem() const
  {
  return nullptr;
  }




SdProject *SdWEditorProject::getProject()
  {
  return mProject;
  }



QString SdWEditorProject::getTitle()
  {
  return mShortName;
  }




QString SdWEditorProject::getIconName()
  {
  return QString(":/pic/drawLine.png");
  }




void SdWEditorProject::onActivateEditor()
  {
  SdWEditor::onActivateEditor();
  }




void SdWEditorProject::cmEditUndo()
  {
  SdWEditor::cmEditUndo();
  }



void SdWEditorProject::cmEditRedo()
  {
  SdWEditor::cmEditRedo();
  }




void SdWEditorProject::onProjectRename(SdProject *prj, const QString shortName)
  {
  if( mProject == prj )
    mShortName = shortName;
  }


