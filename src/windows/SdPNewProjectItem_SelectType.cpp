﻿/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPNewProjectItem_SelectType.h"
#include "SdPNewProjectItem.h"
#include "objects/SdPItemSymbol.h"
#include "objects/SdPItemPart.h"
#include "objects/SdPItemComponent.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdPItemRich.h"
#include "library/SdLibraryStorage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include <QSettings>

static int defaultClass, defaultOrder;

SdPNewProjectItem_SelectType::SdPNewProjectItem_SelectType(SdProjectItemPtr *item, SdProject *prj, QWidget *parent ) :
  QWizardPage(parent),
  mItemPtr(item),
  mProject(prj)
  {

  setTitle( tr("Creation new project object") );
  setSubTitle( tr("Select object type then object creation order") );

  QHBoxLayout *hlay = new QHBoxLayout();
  QVBoxLayout *vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("1.Select object type") ) );
  mObjectType = new QListWidget();
  vlay->addWidget( mObjectType );
  hlay->addLayout( vlay );

  vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("2.Select creation order")) );
  vlay->addWidget( mCreationOrder = new QListWidget() );
  hlay->addLayout( vlay );

  vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Description")) );
  vlay->addWidget( mDescription = new QTextEdit() );
  hlay->addLayout( vlay );

  setLayout( hlay );

  //Fill object classes
  mObjectType->addItem( tr("Sheet") );
  mObjectType->addItem( tr("Construction (PCB)") );
  mObjectType->addItem( tr("Symbol") );
  mObjectType->addItem( tr("Part") );
  mObjectType->addItem( tr("Component") );
  mObjectType->addItem( tr("Text doc") );

  mObjectType->setCurrentRow( defaultClass );
  classChanged( defaultClass );
  orderChanged( defaultOrder );

  connect( mObjectType, &QListWidget::currentRowChanged, this, &SdPNewProjectItem_SelectType::classChanged );
  connect( mCreationOrder, &QListWidget::currentRowChanged, this, &SdPNewProjectItem_SelectType::orderChanged );
  }



bool SdPNewProjectItem_SelectType::validatePage()
  {
  //Create object
  if( *mItemPtr ) delete (*mItemPtr);
  QString name;
  switch( defaultClass ) {
    case 0 :
      *mItemPtr = new SdPItemSheet();
      name = tr("Sheet%1");
      break;
    case 1 :
      *mItemPtr = new SdPItemPlate();
      name = tr("PCB%1");
      break;
    case 2 :
      *mItemPtr = new SdPItemSymbol();
      name = tr("Symbol%1");
      break;
    case 3 :
      *mItemPtr = new SdPItemPart();
      name = tr("Part%1");
      break;
    case 4 :
      *mItemPtr = new SdPItemComponent();
      name = tr("Component%1");
      break;
    case 5 :
      *mItemPtr = new SdPItemRich();
      name = tr("Text doc%1");
      break;
    }

  //Pick up name
  for( int i = 1; i < 10000; i++ )
    if( !mProject->isNameUsed( name.arg(i), (*mItemPtr)->getClass() ) && !SdLibraryStorage::instance()->cfObjectContains( headerUid( (*mItemPtr)->getType(), name.arg(i), SdProjectItem::getDefaultAuthor() ) ) ) {
      (*mItemPtr)->setTitle( name.arg(i), tr("Set object title") );
      break;
      }
  return true;
  }



int SdPNewProjectItem_SelectType::nextId() const
  {
  if( defaultClass <= 4 ) {
    //Create empty object
    if( defaultOrder == 0 )
      return SDP_NPI_NAME;

    //Create copy of existing object
    if( defaultOrder == 1 && defaultClass != 0 )
      return SDP_NPI_COPY;

    //Use 3d part master
    if( defaultOrder == 3 && defaultClass == 3 )
      return SDP_NPI_3D_MASTER;

    //Use master
    return SDP_NPI_MASTER;
    }
  //Create text docs
  return SDP_NPI_NAME;
  }




void SdPNewProjectItem_SelectType::classChanged(int index)
  {
  defaultClass = index;
  mCreationOrder->clear();
  mDescriptions.clear();
  switch(index) {
    case 0 :
      //Schematic sheet
      mCreationOrder->addItem( tr("Empty sheet") );
      mDescriptions.append( tr("Creates empty schematic sheet with no any component or graphics.") );
      mCreationOrder->addItem( tr("Sheet master") );
      mDescriptions.append( tr("Start master which guide you to create schematic sheet") );
//      mCreationOrder->addItem( tr("Sheet with decoration") );
//      mDescriptions.append( tr("Creates empty schematic sheet with inserted sheet form") );
//      mCreationOrder->addItem( tr("Copy of existing sheet") );
//      mDescriptions.append( tr("Creates copy of existing schematic sheet") );
      //mCreationType->addItem( tr(""));
      break;

    case 1 :
      //PCB
      mCreationOrder->addItem( tr("Empty construction") );
      mDescriptions.append( tr("Creates empty construction or pcb") );
      break;

    case 2 :
      //Schematic symbol
      mCreationOrder->addItem( tr("Empty symbol") );
      mDescriptions.append( tr("Creates empty schematic component section symbol with no any pins or graphics.") );
      mCreationOrder->addItem( tr("Copy existing symbol") );
      mDescriptions.append( tr("Creates copy of existing schematic component section") );
      mCreationOrder->addItem( tr("Symbol master") );
      mDescriptions.append( tr("Start master which guide you to create symbol") );
      break;

    case 3 :
      //Part
      mCreationOrder->addItem( tr("Empty part") );
      mDescriptions.append( tr("Creates empty component part with no any pins or graphics.") );
      mCreationOrder->addItem( tr("Copy existing part") );
      mDescriptions.append( tr("Creates copy of existing component part") );
      mCreationOrder->addItem( tr("Part master") );
      mDescriptions.append( tr("Start master which guide you to create part") );
      mCreationOrder->addItem( tr("3D part master") );
      mDescriptions.append( tr("Start master which guide you to create part and its 3d model") );
      break;

    case 4 :
      //Component as combination schematic symbol and part with some param
      mCreationOrder->addItem( tr("Empty component") );
      mDescriptions.append( tr("Creates empty component as pair schematic and part with no any schematic or part.") );
      mCreationOrder->addItem( tr("Copy existing component") );
      mDescriptions.append( tr("Creates copy of existing component") );
      break;

    case 5 :
      //Simple text
      mCreationOrder->addItem( tr("Empty text") );
      //mCreationOrder->addItem( tr("Element list") );
      //mCreationOrder->addItem( tr("Specification") );
      break;

    }
  if( mCreationOrder->count() ) {
    mCreationOrder->setCurrentRow( 0 );
    orderChanged( 0 );
    }
  }




void SdPNewProjectItem_SelectType::orderChanged(int index)
  {
  defaultOrder = index;
  if( index >= 0 && index < mDescriptions.count() )
    mDescription->setText( mDescriptions[index] );
  else
    mDescription->clear();
  }

