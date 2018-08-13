/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Page of creation master with enter ProjectItem name and category selection
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

    SdProjectItemPtr *mItemPtr;   //Pointer to item pointer which store ProjectItem
    SdProject        *mProject;   //Project to which will be appended ProjectItem
    QLabel           *mUnical;    //Message which indicate unical or not ProjectItem name
    QLineEdit        *mName;      //ProjectItem name
    QLineEdit        *mCategory;  //Category name
    bool              mValid;
    QString           mTagPath;
  public:
    SdPNewProjectItem_EnterName(SdProjectItemPtr *item, SdProject *prj, bool categoryOnly, QWidget *parent = nullptr );

    virtual bool validatePage() override;
    virtual bool isComplete() const override;
    virtual int  nextId() const override;
    virtual void initializePage() override;
  public slots:
    //On changed ProjectItem name
    void onTextChanged( const QString name );
  };

#endif // SDPNEWPROJECTITEM_ENTERNAME_H
