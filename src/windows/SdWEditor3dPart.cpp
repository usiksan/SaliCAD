#include "SdWEditor3dPart.h"
#include "SdWCommand.h"

SdWEditor3dPart::SdWEditor3dPart(SdProjectItem *item, QWidget *parent) :
  SdWEditor3d( item, parent )
  {

  }


//SdProjectItem *SdWEditor3dPart::getProjectItem() const
//  {
//  }

void SdWEditor3dPart::onActivateEditor()
  {
  SdWEditor3d::onActivateEditor();

  //Activate menu
  SdWCommand::cmMenuInsertPart3d->setVisible(true);

  //Activate tool bar
  //SdWCommand::barPart->show();

  //Enable finish edit
  SdWCommand::cmObjectEditDisable->setVisible(true);
  }
