/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Listing of creation 3d masters.

  Show list for object type, user select one master and dialog
  execute selected master.
*/
#ifndef SDD3DMASTER_H
#define SDD3DMASTER_H

#include "objects/SdPItemPart.h"
#include "objects/SdProject.h"

#include <QDialog>
#include <QListWidget>
#include <QTextEdit>
#include <QLabel>

class SdWView3d;

class SdD3dMaster : public QDialog
  {
    Q_OBJECT

    SdPItemPart   *mPartPtr;      //!< New created object
    QListWidget   *mMasterType;   //!< Available programm list
    SdWView3d     *mPreview;      //!< 3d preview of master default result
    QTextEdit     *mDescription;  //!< Description widget
    QStringList    mIdList;       //!< Master programm id list
  public:
    SdD3dMaster( SdPItemPart *part, QWidget *parent );


  };

#endif // SDD3DMASTER_H
