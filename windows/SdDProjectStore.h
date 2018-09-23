/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Project store to library dialog

  Allow enter project name in library and project category. On ok pressed project writting to library
*/
#ifndef SDDPROJECTSTORE_H
#define SDDPROJECTSTORE_H

#include "objects/SdProject.h"
#include <QWizardPage>
#include <QLabel>
#include <QLineEdit>


class SdDProjectStore : public QDialog
  {
    Q_OBJECT

    SdProject        *mProject;   //Project which will be stored to library
    QLabel           *mUnical;    //Message which indicate unical or not Project name
    QLineEdit        *mName;      //Project name
  public:
    SdDProjectStore(const QString name, SdProject *prj, QWidget *parent = nullptr );

  public slots:
    void onTextChanged( const QString name );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDPROJECTSTORE_H
