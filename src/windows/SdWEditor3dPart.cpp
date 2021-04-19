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
#include "objects/Sd3dGraphModel.h"
#include "modes/Sd3dModeView.h"
#include "modes/Sd3dModeHorzMove.h"
#include "modes/Sd3dModeHorzRotate.h"
#include "modes/Sd3dModeVertMove.h"
#include "modes/Sd3dModeVertRotate.h"
#include "modes/Sd3dModeFaceColor.h"
#include "import/stl/Sd3dReaderStl.h"
#include "import/vrml/Sd3dReaderVrml.h"

#include <QFileDialog>

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
  mView->update();
  }





void SdWEditor3dPart::cm3dModeView()
  {
  mView->modeSet( new Sd3dModeView() );
  }



void SdWEditor3dPart::cm3dModeHorzMove()
  {
  mView->modeSet( new Sd3dModeHorzMove(mPartPtr) );
  }




void SdWEditor3dPart::cm3dModeHorzRotate()
  {
  mView->modeSet( new Sd3dModeHorzRotate(mPartPtr) );
  }




void SdWEditor3dPart::cm3dModeVertMove()
  {
  mView->modeSet( new Sd3dModeVertMove(mPartPtr) );
  }




void SdWEditor3dPart::cm3dModeVertRotate()
  {
  mView->modeSet( new Sd3dModeVertRotate(mPartPtr) );
  }




void SdWEditor3dPart::cm3dModeFaceColor()
  {
  mView->modeSet( new Sd3dModeFaceColor( mView, mPartPtr ) );
  }



void SdWEditor3dPart::cm3dImportStl()
  {
  //Open dialog to select stl file
  QString title = QFileDialog::getOpenFileName(this, tr("Import STL model"), QString(), tr("STL model files (*%1)").arg(".stl") );

  if( title.isEmpty() ) return;

  importModel( Sd3dReaderStl::importStlFromFile( title ), tr("Import STL model") );
  }



void SdWEditor3dPart::cm3dImportStep()
  {
//  //Open dialog to select STEP file
//  QString title = QFileDialog::getOpenFileName(this, tr("Import STEP model"), QString(), tr("STEP model files (*%1)").arg(".stp") );

//  if( title.isEmpty() ) return;

//  Sd3dStep *step = Sd3dStep::importStepFromFile( title );
//  if( step ) {
//    mItem->getUndo()->begin( tr("Import STEP model"), mItem, true );
//    mItem->insertChild( step, mItem->getUndo() );
//    dirtyProject();
//    mView->update();
//    }
  }



void SdWEditor3dPart::cm3dImportVrml()
  {
  //Open dialog to select VRML file
  QString title = QFileDialog::getOpenFileName(this, tr("Import VRML model"), QString(), tr("VRML model files (*%1)").arg(".wrl") );

  if( title.isEmpty() ) return;

  importModel( Sd3dReaderVrml::importVrmlFromFile( title, this ), tr("Import VRML model") );
  }











//!
//! \brief importModel Does import 3d model into part item. Previous 3d model removed.
//! \param model       Model to insert into part item
//! \param undoTitle   Undo title text
//!
void SdWEditor3dPart::importModel(Sd3dGraph *model, const QString undoTitle)
  {
  if( model != nullptr ) {
    //Initiate operation
    mPartPtr->getUndo()->begin( undoTitle, mPartPtr, true );

    //At first we remove previous model
    //Remove previous 3d objects
    mPartPtr->forEach( dct3D, [this] ( SdObject *obj ) -> bool {
      mPartPtr->deleteChild( obj, mPartPtr->getUndo() );
      return false;
      });

    //Insert new model
    mPartPtr->insertChild( model, mPartPtr->getUndo() );
    dirtyProject();
    mView->update();
    }
  }

