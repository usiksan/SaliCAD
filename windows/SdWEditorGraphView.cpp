/*
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

SdWEditorGraphView::SdWEditorGraphView( QWidget *parent ) :
  SdWEditorGraph( nullptr, parent ),
  mItem(nullptr),
  mOwner(false)
  {

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
  }




SdProjectItem *SdWEditorGraphView::getProjectItem()
  {
  return mItem;
  }




double SdWEditorGraphView::getPPM() const
  {
  return 1.0; //Not used
  }
