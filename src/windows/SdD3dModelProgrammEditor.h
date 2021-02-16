/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programm editor dialog.

  It allow enter, edit and test 3d model programm
*/
#ifndef SDD3DMODELPROGRAMMEDITOR_H
#define SDD3DMODELPROGRAMMEDITOR_H

#include "objects/SdPItemRich.h"
#include "objects/SdPItemPart.h"
#include "SdWView3d.h"

#include <QDialog>
#include <QTextEdit>
#include <QTableWidget>
#include <QLineEdit>

class SdM3dProgramm;
class SdW3dModelProgrammEditor;
class SdW3dModelProgrammHighlighter;

class SdD3dModelProgrammEditor : public QDialog
  {
    Q_OBJECT

    SdPItemPart                    mPart;
    SdM3dProgramm                 *mProgramm;
    SdPItemRich                   *mRich;
    SdW3dModelProgrammEditor      *mTextEdit;
    SdW3dModelProgrammHighlighter *mHighlighter;
    QLineEdit                     *mError;
    QLineEdit                     *mTitle;
    QLineEdit                     *mDescription;
    QTableWidget                  *mParamWidget;
    SdWView3d                     *mPreview;
    bool                           mDirty;
  public:
    SdD3dModelProgrammEditor( const QString id, QWidget *parent );

  private slots:
    void compile();

    void rebuild();

    void parse();
  };

#endif // SDD3DMODELPROGRAMMEDITOR_H
