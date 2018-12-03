/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for get objects from project. Dialog listing existing objects of desired type and allow selected one of it
*/
#ifndef SDDGETPROJECTOBJECT_H
#define SDDGETPROJECTOBJECT_H

#include "SdWEditorGraphView.h"
#include <QDialog>
#include <QList>

namespace Ui {
class SdDGetProjectObject;
}

class SdDGetProjectObject : public QDialog
  {
    Q_OBJECT

    SdWEditorGraphView      *mView;    //View item widget
    QList<SdProjectItemPtr>  mList;
//    SdContainer        *mProject; //Project object selected from
  public:
    explicit SdDGetProjectObject(SdContainer *project, quint64 mask, const QString title, QWidget *parent = 0);
    ~SdDGetProjectObject();

    SdProjectItem *selectedItem() const;
  private:
    Ui::SdDGetProjectObject *ui;

  public:
    static SdProjectItem *getItem( SdContainer *project, quint64 mask, const QString title, QWidget *parent );
  };

#endif // SDDGETPROJECTOBJECT_H
