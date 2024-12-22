/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base editor for 3d views.

  This class is wrapper for real 3d widget, which inserted into this editor.
*/
#include "SdWEditor3d.h"
#include "SdWView3d.h"
#include "SdWCommand.h"
#include "SdD3dModelProgrammEditor.h"
#include "objects/SdProjectItem.h"
#include "objects/Sd3dStep.h"
#include "objects/SdPulsar.h"
#include "objects/Sd3dGraphModel.h"

#include <QVBoxLayout>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QDebug>

SdWEditor3d::SdWEditor3d(SdProjectItem *item, QWidget *parent) :
  SdWEditor( parent ),
  mItem(item)
  {
  QVBoxLayout *box = new QVBoxLayout();
  mView = new SdWView3d( item, this );

  //Link with 2d graphics enable-disable
  mView->setEnable2d( SdWCommand::cm3dShow2d->isChecked() );
  connect( SdWCommand::cm3dShow2d, &QAction::toggled, mView, &SdWView3d::setEnable2d );

  //Link with pad graphics enable-disable
  mView->setEnablePad( SdWCommand::cm3dShowPads->isChecked() );
  connect( SdWCommand::cm3dShowPads, &QAction::toggled, mView, &SdWView3d::setEnablePad );

  box->setContentsMargins( 0, 0, 0, 0 );
  box->addWidget( mView );
  mView->move( 0, 0 );
  setLayout( box );
  mView->fitItem();
  }




void SdWEditor3d::setProjectItem(SdProjectItem *itm)
  {
  mView->setItem( mItem = itm );
  }



void SdWEditor3d::paintEvent(QPaintEvent *event)
  {
//  qDebug() << "paint event 3d";
  event->accept();
//  mView->update();
  }



SdProjectItem *SdWEditor3d::getProjectItem() const
  {
  return mItem;
  }



//This editor is 3d
bool SdWEditor3d::is3d() const
  {
  return true;
  }





void SdWEditor3d::onActivateEditor()
  {
  SdWEditor::onActivateEditor();
  //Update current mode
  SdWCommand::selectMode( mView->mode()->modeId() );
  //Set default status message for activated mode
  SdPulsar::sdPulsar->emitSetStatusMessage( mView->mode()->getStepHelp() );
  mView->setFocus();
  }




void SdWEditor3d::cmEditUndo()
  {
  mView->update();
  }



void SdWEditor3d::keyPressEvent(QKeyEvent *event)
  {
  if( mView ) mView->keyPressEvent(event);
  }



void SdWEditor3d::keyReleaseEvent(QKeyEvent *event)
  {
  if( mView ) mView->keyReleaseEvent(event);
  }



