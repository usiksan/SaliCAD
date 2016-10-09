/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPNEWPROJECTITEM_SELECTTYPE_H
#define SDPNEWPROJECTITEM_SELECTTYPE_H

#include "objects/SdProjectItem.h"
#include "objects/SdProject.h"
#include <QWizardPage>
#include <QListWidget>
#include <QTextEdit>

class SdPNewProjectItem_SelectType : public QWizardPage
  {
    Q_OBJECT

    SdProjectItemPtr *mItemPtr;
    SdProject        *mProject;
    QListWidget      *mObjectType;
    QListWidget      *mCreationOrder;
    QTextEdit        *mDescription;
  public:
    SdPNewProjectItem_SelectType( SdProjectItemPtr *item, SdProject *prj, QWidget *parent = 0);

    virtual bool validatePage() override;
    virtual int  nextId() const override;

  public slots:
    void classChanged( int index );
    void orderChanged( int index );
  };

#endif // SDPNEWPROJECTITEM_SELECTTYPE_H
