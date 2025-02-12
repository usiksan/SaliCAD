/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Listing of creation 3d masters.

  Show list for available 3d masters, user select one master and dialog
  execute selected master. This master also allow to create new master and edit existing master
*/
#ifndef SDPNEWPROJECTITEM_3DMASTER_H
#define SDPNEWPROJECTITEM_3DMASTER_H

#include "objects/SdProjectItem.h"
#include "objects/SdProject.h"
#include "Sd3dModelScript.h"

#include <QWizardPage>
#include <QListWidget>
#include <QTextEdit>
#include <QLabel>

class SdWView3d;

class SdPNewProjectItem_3dMaster : public QWizardPage
  {
    Q_OBJECT

    SdProjectItemPtr   *mItemPtr;      //!< New created object
    SdProject          *mProject;      //!< Project to which will be inserted created object
    QListWidget        *mMasterType;   //!< Available programm list
    SdWView3d          *mPreview;      //!< 3d preview of master default result
    QTextEdit          *mDescription;  //!< Description widget
    Sd3dModelScriptList mModelList;    //!< List of models
  public:
    SdPNewProjectItem_3dMaster( SdProjectItemPtr *item, SdProject *prj, QWidget *parent = nullptr );

    virtual bool validatePage() override;
    virtual int  nextId() const override;
    virtual void initializePage() override;

  private slots:
    //!
    //! \brief onCurrentRowChanged Called on current row in model master list
    //! \param row                 Selected row index
    //!
    void onCurrentRowChanged( int row );
  };

#endif // SDPNEWPROJECTITEM_3DMASTER_H
