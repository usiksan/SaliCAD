/*
  Project "SvLib common library."
  Author
    Alexander Sibilev
  Internet
    www.salilab.com

  Description
    SvDir string which contains directory path
*/
#ifndef SVDIR_H
#define SVDIR_H

#include <QString>

/*!
   \brief The SvDir class It is a string with a directory. Provides movement
                          down and up the directory. Performs the decoration with a final slash.
 */
class SvDir
  {
    QString mCleanedPath; //!< Path without final slash
  public:
    //!
    //! \brief SvDir Path constructor
    //! \param path  Default path. May be with or without final slash
    //!
    SvDir( const QString path = QString() );

    //!
    //! \brief cleanedPath Return path without final slash [Возвращает путь без завершающего слэша]
    //! \return            Path without final slash [Путь без завершающего слэша]
    //!
    QString cleanedPath() const { return mCleanedPath; }

    //!
    //! \brief slashedPath Return path with final slash [Возвращает путь с завершающим слэшем]
    //! \return            Path with final slash [Путь с завершающим слэшем]
    //!
    QString slashedPath() const { return mCleanedPath + QStringLiteral("/"); }

    //!
    //! \brief set  Set a new path. The path being set is cleared of the trailing slash
    //!             since internal storage is without a trailing slash
    //!             [Установить новый путь. Устанавливаемый путь очищается от завершающего слэша
    //!             так как внутреннее хранение - без завершающего слэша]
    //! \param path New path [Новый путь]
    //!
    void    set( QString path );

    //!
    //! \brief up Moves up the directory tree by one step [Поднимается по дереву директориев вверх на одну ступень]
    //!
    void    up();

    //!
    //! \brief cd   Moves down the directory tree by one step [Опускается по дереву директориев вниз на одну ступень.]
    //! \param name The subdirectory in this directory that you need to enter [Поддиректорий в данном директории, в который нужно зайти]
    //!
    void    cd( const QString name );
  };

#endif // SVDIR_H
