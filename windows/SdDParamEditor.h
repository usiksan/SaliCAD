#ifndef SDDPARAMEDITOR_H
#define SDDPARAMEDITOR_H

#include "objects/SdContainer.h"

class SdDParamEditor : public QDialog
  {
    Q_OBJECT

    SdContainer          *mItem;              //Editing container with params
    SdUndo               *mUndo;              //Undo for editing

    QTableWidget         *mParamTable;        //Table of user params
    QPushButton          *mParamAdd;
    QPushButton          *mParamAddDefault;
    QPushButton          *mParamDelete;
    QPushButton          *mParamCopy;
  public:
    SdDParamEditor();
  };

#endif // SDDPARAMEDITOR_H
