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
#include <QVBoxLayout>

SdWEditorProject::SdWEditorProject(SdProject *prj, const QString shortName) :
  SdWEditor(),
  mProject(prj),
  mShortName(shortName)
  {
  mTitle = new QTextBrowser();
  QVBoxLayout *box = new QVBoxLayout();
  box->addWidget( mTitle );
  setLayout( box );

  connect( SdPulsar::sdPulsar, &SdPulsar::renameProject, this, &SdWEditorProject::onProjectRename );

  //Build project title
  onProjectRename( prj, shortName );
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
  return QString(":/pic/iconProject.png");
  }




void SdWEditorProject::onActivateEditor()
  {
  SdWEditor::onActivateEditor();
  SdPulsar::sdPulsar->emitHighlightProject( mProject );
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
  if( mProject == prj ) {
    mShortName = shortName;

    //Build visual title
    QString htm = QString("<h1 align=\"center\">%1</h1>").arg( mShortName );
    mTitle->setHtml( htm );
    }
  }


