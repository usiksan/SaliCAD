/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Model interface to project
*/

#ifndef SDPROJECTMODEL_H
#define SDPROJECTMODEL_H

#include <QAbstractItemModel>

class SdProject;

class SdProjectModel : public QAbstractItemModel
  {
    SdProject *mProject;
  public:
    SdProjectModel();
  };

#endif // SDPROJECTMODEL_H
