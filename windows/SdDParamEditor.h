/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for param editing and viewing from container.
*/
#ifndef SDDPARAMEDITOR_H
#define SDDPARAMEDITOR_H

#include "objects/SdContainer.h"

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>

class SdProject;

class SdDParamEditor : public QDialog
  {
    Q_OBJECT

    SdContainer          *mItem;              //Editing container with params
    SdUndo               *mUndo;              //Undo for editing
    SdProject            *mProject;           //Project if item is project

    QTableWidget         *mParamTable;        //Table of user params
    QPushButton          *mParamAdd;
    QPushButton          *mParamAddDefault;
    QPushButton          *mParamDelete;
    QPushButton          *mParamCopy;
    QPushButton          *mParamFields;       //Accum fields from all sheets

    bool                  mEditEnable;
  public:
    SdDParamEditor(const QString title, SdContainer *cnt, bool editEnable, QWidget *parent = nullptr );
    //~SdDParamEditor() override;

  public slots:
    void onParamChanged( int row, int column );

    void paramAdd();
    void paramAddDefault();
    void paramDelete();
    void paramCopy();
    void paramFields();

  private:
    void fillParams();
    void paramAppend(int row, const QString key, const QString value );
    void paramAddInt( const QString key, const QString value = QString() );
    void dirtyProject();
  };

#endif // SDDPARAMEDITOR_H
