/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdObjectFactory.h"

//SdObjectFactory::SdObjectFactory()
//  {

//  }

QString SdObjectFactory::mLibraryPath;

void SdObjectFactory::setLibraryPath(const QString path)
  {
  //Save changes to current data base
  if( mDirty )

  mLibraryPath = path;
  }
