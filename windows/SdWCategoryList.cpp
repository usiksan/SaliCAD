/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Widget to display and navigate on category hierarchy
*/

#include "objects/SdEnvir.h"
#include "SdWCategoryList.h"

static QString categoryPath;

SdWCategoryList::SdWCategoryList(QWidget *parent) :
  QListWidget(parent)
  {
  if( categoryPath.isEmpty() )
    categoryPath = sdEnvir->mCategoryPath;
  //Fill with contens
  fill();
  }




//Fill list with contens of current category path
void SdWCategoryList::fill()
  {
  addItem( tr("All") );
  }
