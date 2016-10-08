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
    SdProjectModel( SdProject *project, QObject *parent = 0 );
    ~SdProjectModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  };

#endif // SDPROJECTMODEL_H
