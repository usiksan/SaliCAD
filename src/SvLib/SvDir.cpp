/*
  Project  "SvLib - common library"
  Author
    Alexander Sibilev
  Internet
    www.saliLab.com

  Description
    SvDir string which contains directory path
*/
#include "SvDir.h"

//!
//! \brief SvDir Path constructor
//! \param path  Default path. May be with or without final slash
//!
SvDir::SvDir(const QString path)
  {
  set( path );
  }



//!
//! \brief set  Set a new path. The path being set is cleared of the trailing slash
//!             since internal storage is without a trailing slash
//!             [Установить новый путь. Устанавливаемый путь очищается от завершающего слэша
//!             так как внутреннее хранение - без завершающего слэша]
//! \param path New path [Новый путь]
//!
void SvDir::set(QString path)
  {
  //Если путь завершается слэшем, то удаляем его
  //в противном случае присваиваем без изменений
  if( path.endsWith( QChar('/')) )
    mCleanedPath = path.left( path.length() - 1 );
  else
    mCleanedPath = path;
  }



//!
//! \brief up Moves up the directory tree by one step [Поднимается по дереву директориев вверх на одну ступень]
//!
void SvDir::up()
  {
  //Ищем крайний справа слэш
  int slash = mCleanedPath.lastIndexOf( QChar('/') );
  //Если нашли, то отсекаем все, что справа от этого символа и сам символ
  if( slash > 0 )
    mCleanedPath = mCleanedPath.left( slash );
  }



//!
//! \brief cd   Moves down the directory tree by one step [Опускается по дереву директориев вниз на одну ступень.]
//! \param name The subdirectory in this directory that you need to enter [Поддиректорий в данном директории, в который нужно зайти]
//!
void SvDir::cd(const QString name)
  {
  //Добавляем к пути имя поддиректория
  set( slashedPath() + name );
  }


