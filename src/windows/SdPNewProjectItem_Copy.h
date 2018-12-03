/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Creation copy of existing object.

  Allow to select object for cloning
*/
#ifndef SDPNEWPROJECTITEM_COPY_H
#define SDPNEWPROJECTITEM_COPY_H

#include "objects/SdProjectItem.h"
#include "objects/SdProject.h"
#include <QWizardPage>
#include <QLineEdit>
#include <QLabel>


class SdPNewProjectItem_Copy : public QWizardPage
  {
    Q_OBJECT

    QLineEdit          *mCopyName;
    //QString             mCopyUid;
    SdProjectItemPtr   *mItem;
    SdProject          *mProject;
    SdProjectItem      *mCopyItem;
    SdProject          *mCopyProject;
    bool                mCopyOwner;
  public:
    SdPNewProjectItem_Copy( SdProjectItemPtr *itemPtr, SdProject *prj, QWidget *parent = nullptr );

    // QWizardPage interface
  public:
    virtual void initializePage() override;
    virtual bool validatePage() override;
    virtual int  nextId() const override;

  private:
    //Clear copy item and delete all owned copy objects
    void clearCopyItem();

    //Update copy item information
    void copyItemUpdate();

  public slots:
    void onCopyFromFile();
  };

#endif // SDPNEWPROJECTITEM_COPY_H
