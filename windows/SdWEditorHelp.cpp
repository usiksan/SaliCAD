#include "SdWEditorHelp.h"
#include "objects/SdPulsar.h"

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
  }


SdProjectItem *SdWEditorHelp::getProjectItem() const
  {
  return nullptr;
  }


void SdWEditorHelp::onActivateEditor()
  {
  }
