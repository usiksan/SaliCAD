/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Listing of creation masters.

  Show list for object type, user select one master and dialog
  execute selected master.
*/
#ifndef SDPNEWPROJECTITEM_MASTER_H
#define SDPNEWPROJECTITEM_MASTER_H

#include "objects/SdProjectItem.h"
#include "objects/SdProject.h"
#include <QWizardPage>
#include <QListWidget>
#include <QTextEdit>
#include <QLabel>
#include <functional>

typedef std::function<bool(SdProjectItem *item, QWidget *parent)> SdMasterFun;

class SdPNewProjectItem_Master : public QWizardPage
  {
    Q_OBJECT

    SdProjectItemPtr   *mItemPtr;
    SdProject          *mProject;
    QListWidget        *mMasterType;
    QLabel             *mImage;
    QTextEdit          *mDescription;
    QStringList         mDescriptions;
    QStringList         mImages;
    QList<SdMasterFun>  mFuncs;
  public:
    SdPNewProjectItem_Master( SdProjectItemPtr *item, SdProject *prj, QWidget *parent = nullptr );

    virtual bool validatePage() override;
    virtual int  nextId() const override;
    virtual void initializePage() override;

  private:
    void addMaster( QString title, QString descr, QString img, SdMasterFun fun );
  };

#endif // SDPNEWPROJECTITEM_MASTER_H
