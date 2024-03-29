﻿/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Widget for viewing any object
*/
#include "SdWEditorGraphView.h"
#include "SdWCommand.h"
#include "objects/SdPulsar.h"
#include "library/SdLibraryStorage.h"
#include "SdPMasterList.h"
#include "SdPExport_Dxf.h"

#include <QDebug>
#include <QPaintEvent>

SdWEditorGraphView::SdWEditorGraphView(SdProjectItem *item, QWidget *parent) :
  SdWEditorGraph( item, parent ),
  mItem(item),
  mOwner(false)
  {
  if( item != nullptr && item->getClass() & (dctPart | dctPlate) ) {
    mGrid.set( 2500, 2500 );
    mScale.scaleSet( 0.01 );
    }
  else {
    mGrid.set( 25, 25 );
    mScale.scaleSet( 1.0 );
    }
  //Fill item in view
  //cmViewFit();
  }




SdWEditorGraphView::SdWEditorGraphView( QWidget *parent ) :
  SdWEditorGraph( nullptr, parent ),
  mItem(nullptr),
  mOwner(false)
  {
  mGrid.set(1,1);
  mScale.scaleSet(1.0);
  }




SdWEditorGraphView::~SdWEditorGraphView()
  {
  if( mItem != nullptr && mOwner )
    delete mItem;
  }



//Set new item
void SdWEditorGraphView::setItem( SdProjectItem *item, bool owner )
  {
  //Delete previous item
  if( mItem != nullptr ) {
    if( mOwner )
      delete mItem;
    mItem = nullptr;
    }

  //Set new item
  mItem = item;
  mOwner = owner;

  if( mItem ) {
    //Fill item in view
    cmViewFit();
    }
  else update();
  }




//Set new item by item id
void SdWEditorGraphView::setItemById(const QString id)
  {
  setItem( sdObjectOnly<SdProjectItem>( SdLibraryStorage::instance()->cfObjectGet(id) ), true );
  }







SdProjectItem *SdWEditorGraphView::getProjectItem() const
  {
  return mItem;
  }




void SdWEditorGraphView::onActivateEditor()
  {
  SdWEditorGraph::onActivateEditor();

  //Activate menu
  SdWCommand::cmObjectEditEnable->setVisible(true);

  //Activate tool bar
  SdWCommand::barView->show();
  }





//Export command
void SdWEditorGraphView::cmFileExport()
  {
  if( mItem ) {
    //Wizard
    QWizard wizard(this);
    //Fill it with pages
    SdPMasterList *master = new SdPMasterList( tr("Export object"), tr("Select export master"), &wizard );
    wizard.setPage( 0,   master );
    wizard.setPage( 1,   new SdPExport_Dxf( mItem, 1, master, &wizard) );
    wizard.exec();
    }
  }







void SdWEditorGraphView::paintEvent(QPaintEvent *event)
  {
  event->accept();
  paintProcess( true );
  }





bool SdWEditorGraphView::showFields()
  {
//  if( mItem && (mItem->getClass() & (dctSymbol)))
  return true;
  }
