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

#include "SdExpression.h"

#include <QDialog>
#include <QTextEdit>
#include <QTableWidget>

namespace Ui {
  class SdDExpressionEdit;
}

class SdDExpressionEdit : public QDialog
  {
    Q_OBJECT

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
