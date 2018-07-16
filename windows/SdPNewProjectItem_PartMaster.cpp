/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdPNewProjectItem_PartMaster.h"

SdPNewProjectItem_PartMaster::SdPNewProjectItem_PartMaster( SdProjectItem *item, SdProject *prj, QWidget *parent ) :
  QWizardPage( parent ),
  mItem(item),
  mProject(prj)
  {

  }

bool SdPNewProjectItem_PartMaster::validatePage()
  {

  }

int SdPNewProjectItem_PartMaster::nextId() const
  {

  }

void SdPNewProjectItem_PartMaster::onTextChanged(const QString name)
  {

  }

