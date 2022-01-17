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
#include "script/SdScriptProgramm.h"
#include "SdWView3d.h"
#include "SdWHelp.h"

#include <QDialog>
#include <QTextEdit>
#include <QTableWidget>
#include <QLineEdit>

class SdScriptProgramm;
class SdWScriptEditor;
class SdWScriptHighlighter;

class SdD3dModelProgrammEditor : public QDialog
  {
    Q_OBJECT

    SdPItemPart           mPart;        //!< Part for preview
    SdScriptProgrammPtr   mProgramm;    //!< Parsed programm ready for executing
    SdPItemRich          *mRich;        //!< Programm source code object
    SdWScriptEditor      *mTextEdit;    //!< Source code editor with highlight
    SdWScriptHighlighter *mHighlighter; //!< Highlight for code editor
    QLineEdit            *mError;       //!< Source code parsing error if any
    QLineEdit            *mTitle;       //!< Source code object title (shown as master name)
    QLineEdit            *mDescription; //!< Source code short description (shown as code description)
    QTableWidget         *mParamWidget; //!< Table with param list for this programm. Table filled automatic when parsing source code
    SdWView3d            *mPreview;     //!< Preview of 3d part model builded with current params
    SdWHelp              *mHelp;        //!< Help widget with help on source code editor and its predefined functions
    bool                  mDirty;       //!< true when source code changed
    bool                  mActive;      //!< Lock for recursive change param table contents
  public:
    SdD3dModelProgrammEditor( const QString id, QWidget *parent );
    ~SdD3dModelProgrammEditor();

  signals:
    //!
    //! \brief parseCompleted Emited when parse compelete
    //!
    void parseCompleted();

  private slots:
    //!
    //! \brief compile Compile 3d model programm and receiv compilation errors
    //!
    void compile();

    //!
    //! \brief rebuild Rebuild resultat model on model param changed
    //!
    void rebuild();

    //!
    //! \brief parse Parse programm when programm text changed. It need for
    //!              programm color highlighting and autocompletion function
    //!
    void parse();

    //!
    //! \brief save Save editing programm to library
    //!
    void save();

    //!
    //! \brief onCellClicked Called when user clicked on mParamWidget cell
    //! \param row           Row on which was clicked
    //! \param column        Column on which was clicked
    //!
    void onCellClicked( int row, int column );

  protected:
    //We test if programm not saved
    virtual void closeEvent( QCloseEvent *event ) override;


    // QDialog interface
  public slots:
    virtual void reject() override;
  };

#endif // SDD3DMODELPROGRAMMEDITOR_H
