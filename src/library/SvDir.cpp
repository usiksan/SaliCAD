/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SvDir.h"

SvDir::SvDir(const QString path)
  {
  set( path );
  }



void SvDir::set(QString path)
  {
  if( path.endsWith( QChar('/')) )
    mCleanedPath = path.left( path.length() - 1 );
  else
    mCleanedPath = path;
  }



void SvDir::up()
  {
  int slash = mCleanedPath.lastIndexOf( QChar('/') );
  if( slash > 0 )
    mCleanedPath = mCleanedPath.left( slash );
  }



void SvDir::cd(const QString name)
  {
  set( slashedPath() + name );
  }


