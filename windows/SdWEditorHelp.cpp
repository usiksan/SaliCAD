/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Help system in editor space
*/
#include "SdWEditorHelp.h"
#include "objects/SdPulsar.h"
#include "SdWCommand.h"

#include <QVBoxLayout>

SdWEditorHelp::SdWEditorHelp(QWidget *parent) :
  SdWEditor(parent)
  {
  //Create help widget
  mHelp = new SdWHelp(this);

  //And fit it to full editor space
  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget( mHelp );
  box->setMargin(0);

  setLayout( box );

  connect( SdPulsar::sdPulsar, &SdPulsar::helpTopic, mHelp, &SdWHelp::helpTopic );

  connect( mHelp, &SdWHelp::backwardAvailable, SdWCommand::cmHelpBackward, &QAction::setEnabled );
  connect( mHelp, &SdWHelp::forwardAvailable, SdWCommand::cmHelpForward, &QAction::setEnabled );
  }



void SdWEditorHelp::helpBackward()
  {
  mHelp->backward();
  }




void SdWEditorHelp::helpForward()
  {
  mHelp->forward();
  }


SdProjectItem *SdWEditorHelp::getProjectItem() const
  {
  return nullptr;
  }


void SdWEditorHelp::onActivateEditor()
  {
  //SdWEditor::onActivateEditor();

  SdWCommand::barHelp->show();
  SdWCommand::cmHelpBackward->setEnabled( mHelp->isBackwardAvailable() );
  SdWCommand::cmHelpForward->setEnabled( mHelp->isForwardAvailable() );
  }
