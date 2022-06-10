#include "SdWEditor3dPartView.h"
#include "SdWCommand.h"
#include "SdWView3d.h"
#include "SdD3dModelProgrammEditor.h"
#include "objects/Sd3dGraphModel.h"

SdWEditor3dPartView::SdWEditor3dPartView(SdPItemPart *item, QWidget *parent) :
  SdWEditor3d( item, parent ),
  mPartPtr(item)
  {

  }

void SdWEditor3dPartView::onActivateEditor()
  {
  SdWEditor3d::onActivateEditor();

  //Activate menu
  SdWCommand::cmMenuViewPart3d->setVisible(true);

  //Activate tool bar
  SdWCommand::barPart3dView->show();

  //Enable start edit
  SdWCommand::cmObjectEditEnable->setVisible(true);
  }

void SdWEditor3dPartView::cm3dProgram()
  {
  SdD3dModelProgrammEditor programmEditor( mPartPtr->getTitle(), mPartPtr->modelConst()->script(), this );
  programmEditor.exec();
  }
