#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>


class SdWScriptHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

    QStringList mKeyWords;
    QStringList mVariableNameList;
    QStringList mFunctionNameList;

    QString     mLink; //Идентификатор для перехода по линку
    int         mLinkStart;
  public:

    SdWScriptHighlighter(QTextDocument *parent = nullptr);

    void setNameLists( const QStringList &variableNameList, const QStringList &functionNameList )
      {
      mVariableNameList = variableNameList;
      mFunctionNameList = functionNameList;
      }

    QStringList names() const { return mKeyWords + mVariableNameList + mFunctionNameList; }

    bool        isFunction( const QString &id ) const { return mFunctionNameList.contains(id); }

  protected:
    void highlightBlock(const QString &text);

  private:

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat mKeywordFormat;           //!< Ключевые слова
    QTextCharFormat mSingleLineCommentFormat; //!< Однострочные коментарии
    QTextCharFormat mMultiLineCommentFormat;  //!< Многострочный коментарий
    QTextCharFormat mQuotationFormat;         //!< Строки и символы
    QTextCharFormat mIdentFormat;             //!< Для идентификаторов глобальных
    QTextCharFormat mFuncFormat;              //!< Для функций глобальных
    QTextCharFormat mMacroFormat;             //!< Макрос
    QTextCharFormat mPreprocessFormat;        //!< Препроцессор
    QTextCharFormat mDecimalFormat;           //!< Десятичные числа
    QTextCharFormat mOctalFormat;             //!< Восьмиричные числа
    QTextCharFormat mHexFormat;               //!< Шестнадцатиричные числа
    QTextCharFormat mDebugFormat;             //!< Текущая отлаживаемая строка
    QTextCharFormat mLinkFormat;              //!< Ссылка на определение переменной

    int SkipQuotation( const QString &text, int index, int count );
    int MultiLineComment( const QString &text, int index, int count );
  signals:

  public slots:
    void setLink( const QString link, int linkStart );

  };

#endif // HIGHLIGHTER_H
