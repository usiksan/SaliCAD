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
#include "objects/SdProjectItem.h"
#include "objects/Sd3dStl.h"

#include <QVBoxLayout>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QFileDialog>

SdWEditor3d::SdWEditor3d(SdProjectItem *item, QWidget *parent) :
  SdWEditor( parent ),
  mItem(item)
  {
  QVBoxLayout *box = new QVBoxLayout();
  mView = new SdWView3d( item, this );
  box->setMargin(0);
  box->addWidget( mView );
  mView->move( 0, 0 );
//  mView->resize( 300, 300 );
  //setViewport( mView );
  setLayout( box );
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


//void SdWEditor3d::onActivateEditor()
//  {
//  }




void SdWEditor3d::cm3dStlImport()
  {
  //Open dialog to select stl file
  QString title = QFileDialog::getOpenFileName(this, tr("Import STL model"), QString(), tr("STL model files (*%1)").arg(".stl") );

  if( title.isEmpty() ) return;

  Sd3dStl *stl = Sd3dStl::importStlFromFile( title );
  if( stl ) {
    mItem->getUndo()->begin( tr("Import STL model"), mItem );
    mItem->insertChild( stl, mItem->getUndo() );
    dirtyProject();
    mView->update();
    }
  }