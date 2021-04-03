/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Editor for 3d part.

  This class is wrapper for real 3d widget, which inserted into this editor.
*/
#include "SdWEditor3dPart.h"
#include "SdWCommand.h"
#include "SdD3dMaster.h"
#include "SdWView3d.h"
#include "modes/Sd3dModeHorzMove.h"

SdWEditor3dPart::SdWEditor3dPart(SdPItemPart *item, QWidget *parent) :
  SdWEditor3d( item, parent ),
  mPartPtr(item)
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
  SdWCommand::barPart3d->show();

  //Enable finish edit
  SdWCommand::cmObjectEditDisable->setVisible(true);
  }




void SdWEditor3dPart::cm3dMaster()
  {
  SdD3dMaster master( mPartPtr, this );
  master.exec();
  }




void SdWEditor3dPart::cm3dModeHorzMove()
  {
  mView->modeSet( new Sd3dModeHorzMove(mPartPtr) );
  }



void SdWEditor3dPart::cm3dModeHorzRotate()
    {
    }

void SdWEditor3dPart::cm3dModeVertMove()
    {
    }

void SdWEditor3dPart::cm3dModeVertRotate()
    {
    }

void SdWEditor3dPart::cm3dModeFaceColor()
    {
    }
