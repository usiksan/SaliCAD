/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Intro page with previous files list, help and guide access and some fast commands
*/
#include "SdWEditorIntro.h"
#include "objects/SdPulsar.h"
#include "SdWCommand.h"

#include <QVBoxLayout>

SdWEditorIntro::SdWEditorIntro(SdWMain *main, QWidget *parent) :
  SdWEditor(parent)
  {
  //Create help widget
  mHelp = new SdWHelp(main);

  //And fit it to full editor space
  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget( mHelp );
  box->setMargin(0);

  setLayout( box );
  }



SdProjectItem *SdWEditorIntro::getProjectItem() const
  {
  return nullptr;
  }




void SdWEditorIntro::onActivateEditor()
  {

  }
