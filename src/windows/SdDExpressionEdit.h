/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for edit and test sheet expressions
*/
#ifndef SDDEXPRESSIONEDIT_H
#define SDDEXPRESSIONEDIT_H

#include "SdWHelp.h"
#include "script/SdScriptProgramm.h"

#include <QDialog>
#include <QTextEdit>
#include <QTableWidget>


class SdScriptProgramm;
class SdWScriptEditor;
class SdWScriptHighlighter;
class SdPItemSheet;

namespace Ui {
  class SdDExpressionEdit;
}

class SdDExpressionEdit : public QDialog
  {
    Q_OBJECT

    SdPItemSheet         *mSheet;       //!< Part for preview
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

    SdExpression        mExpression; //!< Expression parser and calculator
    QTextEdit          *mText;       //!< Text of expression
    QTableWidget       *mTable;      //!< Table where param and result of expression calculation displays
    bool                mParseActive;
  public:
    explicit SdDExpressionEdit( QStringList list, QWidget *parent = nullptr);
    ~SdDExpressionEdit();

    QString textGet() const { return mText->toPlainText(); }
  private:
    Ui::SdDExpressionEdit *ui;

  private slots:
    void parse();
    void updateResult();
  };

#endif // SDDEXPRESSIONEDIT_H
