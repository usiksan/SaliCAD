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
#include "SdWCommand.h"
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
  //Replace anchor clicked
  mTitle->setOpenLinks(false);
  connect( mTitle, &QTextBrowser::anchorClicked, this, &SdWEditorProject::onLinkClick );

  //Build project title
  visualBuild();
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




QString SdWEditorProject::getToolTip()
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
  visualBuild();
  }



void SdWEditorProject::cmEditRedo()
  {
  SdWEditor::cmEditRedo();
  visualBuild();
  }




void SdWEditorProject::onProjectRename(SdProject *prj, const QString shortName)
  {
  if( mProject == prj ) {
    mShortName = shortName;

    visualBuild();
    }
  }




void SdWEditorProject::onLinkClick(QUrl url)
  {
  QString path = url.toString();
  if( path.startsWith("edit:") ) {
    SdWCommand::cmProjectParam->trigger();
    return;
    }
  }




void SdWEditorProject::visualBuild()
  {
  //Build visual title
  QString htm = QString("<h1 align=\"center\">%1</h1>").arg( mShortName );
  //Button to change params
  htm.append( "<hr><p><a href=\"edit:\">" ).append( tr("Edit parameters table") ).append("</a> </p><hr>");
  SdStringMap map = mProject->paramTable();
  if( map.count() ) {
    //Build visual parameters table representation
    htm.append( "<h2 align=\"center\">" ).append( tr("Project parameters table") ).
        append( "</h2><table>" ).append("<tr><td><h4>").append( tr("Parameter name") ).append("</h4></td>").
        append( "<td><h4>").append( tr("Parameter value") ).append("</h4></td></tr>");
    for( auto iter = map.cbegin(); iter != map.cend(); iter++ ) {
      htm.append( "<tr><td>" ).append( iter.key() ).append("</td><td>").append( iter.value() ).append( "</td></tr>" );
      }
    htm.append( "</table>" );
    }
  mTitle->setHtml( htm );
  }


