/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPNEWPROJECTITEM_ENTERNAME_H
#define SDPNEWPROJECTITEM_ENTERNAME_H

#include "objects/SdProjectItem.h"
#include "objects/SdProject.h"
#include <QWizardPage>
#include <QLabel>
#include <QLineEdit>

class SdPNewProjectItem_EnterName : public QWizardPage
  {
    Q_OBJECT

    SdProjectItemPtr *mItemPtr;
    SdProject        *mProject;
    QLabel           *mUnical;
    QLineEdit        *mName;
    QLineEdit        *mCategory;
    bool              mValid;
    QString           mTagPath;
  public:
    SdPNewProjectItem_EnterName( SdProjectItemPtr *item, SdProject *prj, QWidget *parent = nullptr );

    virtual bool validatePage() override;
    virtual bool isComplete() const override;
    virtual int  nextId() const override;
    virtual void initializePage() override;
  public slots:
    void onTextChanged( const QString name );
  };

#endif // SDPNEWPROJECTITEM_ENTERNAME_H
