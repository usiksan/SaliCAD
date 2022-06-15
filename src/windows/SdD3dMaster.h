/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Listing of creation 3d masters. Used to append 3d model to existing 2d projection.

  Show list for object type, user select one master and dialog
  execute selected master.
*/
#ifndef SDD3DMASTER_H
#define SDD3DMASTER_H

#include "objects/SdPItemPart.h"
#include "objects/SdProject.h"
#include "Sd3dModelScript.h"

#include <QDialog>
#include <QListWidget>
#include <QTextEdit>
#include <QLabel>

class SdWView3d;

class SdD3dMaster : public QDialog
  {
    Q_OBJECT

    SdPItemPart        *mPartPtr;      //!< Object to append 3d model
    SdPItemPart        *mPreviewPart;  //!< Part object to preview model
    QListWidget        *mMasterType;   //!< Available programm list
    SdWView3d          *mPreview;      //!< 3d preview of master default result
    QTextEdit          *mDescription;  //!< Description widget
    Sd3dModelScriptList mModelList;    //!< List of models
  public:
    SdD3dMaster( SdPItemPart *part, QWidget *parent );
    ~SdD3dMaster();

  private slots:
    //!
    //! \brief onCurrentRowChanged Called on current row in model master list
    //! \param row                 Selected row index
    //!
    void onCurrentRowChanged( int row );

  private:
    void initializePage();

  };

#endif // SDD3DMASTER_H
