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
#include "SdExpressionRefMap.h"
#include "script/SdScriptProgramm.h"

#include <QDialog>
#include <QTextEdit>
#include <QTableWidget>


class SdScriptProgramm;
class SdWScriptEditor;
class SdWScriptHighlighter;

class SdDExpressionEdit : public QDialog
  {
    Q_OBJECT

    SdExpressionRefMap   *mRefMap;      //!< Reference map to visual table
    SdScriptProgrammPtr   mProgramm;    //!< Parsed programm ready for executing
    SdWScriptEditor      *mTextEdit;    //!< Source code editor with highlight
    SdWScriptHighlighter *mHighlighter; //!< Highlight for code editor
    QLineEdit            *mError;       //!< Source code parsing error if any
    QTableWidget         *mParamWidget; //!< Table with param and result list for this programm. Table filled automatic when parsing source code
    SdWHelp              *mHelp;        //!< Help widget with help on source code editor and its predefined functions
    bool                  mDirty;       //!< true when source code changed
    bool                  mActive;      //!< Lock for recursive change param table contents
  public:
    explicit SdDExpressionEdit( const QString &script, QWidget *parent = nullptr);
    virtual ~SdDExpressionEdit();

    QString scriptGet() const;

  private slots:
    void parse();
    void recalculation();
  };

#endif // SDDEXPRESSIONEDIT_H
