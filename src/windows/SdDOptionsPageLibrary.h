/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Library
*/
#ifndef SDDOPTIONSPAGELIBRARY_H
#define SDDOPTIONSPAGELIBRARY_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>

class SdDOptionsPageLibrary : public QWidget
  {
    Q_OBJECT
    QLineEdit *mServerRepo;           //!< Remote library server ip
    QLineEdit *mAuthorName;           //!< Author name
    QLineEdit *mObjectsCount;         //!< Objects count in library
    QLineEdit *mLibraryPath;          //!< Library path [Каталог библиотек]
    QCheckBox *mAutoUpload;           //!< Enable automatic upload objects into remote library

  public:
    explicit SdDOptionsPageLibrary(QWidget *parent = nullptr);

  signals:

  };

#endif // SDDOPTIONSPAGELIBRARY_H
