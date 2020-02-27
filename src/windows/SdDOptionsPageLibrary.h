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

class SdDOptionsPageLibrary : public QWidget
  {
    Q_OBJECT
    QLineEdit *mServerIP;             //Remote library server ip
    QLineEdit *mAuthorName;           //Author name
    QLineEdit *mObjectsCount;         //Objects count in library
    QLineEdit *mLibraryPath;          //Library path [Каталог библиотек]

  public:
    explicit SdDOptionsPageLibrary(QWidget *parent = nullptr);

  signals:

  };

#endif // SDDOPTIONSPAGELIBRARY_H
