/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for library management
*/
#ifndef SDDLIBRARY_H
#define SDDLIBRARY_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>

class SdDLibrary : public QDialog
  {
    Q_OBJECT

    QLineEdit *mServerRepo;           //!< Remote library server ip
    QLineEdit *mLibraryPath;          //!< Local Library path [Каталог библиотек]
    //QLineEdit *mCloudLibraryPath;
  public:
    SdDLibrary();
  };

#endif // SDDLIBRARY_H
