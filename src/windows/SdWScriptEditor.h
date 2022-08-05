/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  This widget is script editor with highlight
*/
#ifndef SDWSCRIPTEDITOR_H
#define SDWSCRIPTEDITOR_H

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QDateTime>
#include <QListWidget>
#include <QTimer>
#include <QScrollBar>

class SdWScriptHighlighter;
class LineNumberArea;

//!
//! \brief The BlockNode class Local block for script. It contains line indexes and symbol indexes of begin and end of block.
//!                            Embed level ised for building graph of blocks, which used for right autoident
//!
//! [Локальный блок для С, содержащий начало(строку и символ), конец, уровень вложенности в текстовом документе
//! используется для построения графа блоков, что затем может использоваться для правильного автоиндента]
class BlockNode: public QObject {
    Q_OBJECT
  public:
    int  mStartLineNumber;   //!< Line index of start of block [строка начала блока]
    int  mEndLineNumber;     //!< Line index of end of block [строка окончания блока]
    int  mLevel;             //!< Embed level [уровень вложенности]
    int  mStartIndex;        //!< Symbol index of start of block [начальный индекс]
    int  mEndIndex;          //!< Symbol index of end of block [конечный индекс]
    bool mSingle;            //!< True if block is single - without {}, for example if() i++; [является ли блок одиночным - без {}, например if () i++;]
  public:
    explicit BlockNode(BlockNode* parent): QObject(parent){}

    //!
    //! \brief containsLine Test if block contains line with number lineNum [содержит ли блок строку]
    //! \param lineNum      Tested line number
    //! \return             true if block contains line with number lineNum
    //!
    bool containsLine(int lineNum)const { if (mSingle) return lineNum <= mEndLineNumber && lineNum > mStartLineNumber; else return lineNum < mEndLineNumber && lineNum > mStartLineNumber; }

    //!
    //! \brief containsIndex Test if block contains symbol with index [содержит ли блок символ]
    //! \param index         Tested symbol index
    //! \return              true if block contains symbol with index
    //!
    bool containsIndex(int index)const { return index <= mEndIndex && index >= mStartIndex; }
  };


///
/// \brief The ScrollBarWithMarkers class полоса прокрутки с метками поиска
/// mHighlightedLines - список <начало, высота>, значения от 0 до 1 (нормализовано по размеру документа в пискелях)
///
class ScrollBarWithMarkers : public QScrollBar {
    Q_OBJECT

    QList<QPair<double, double>> mHighlightedLines;
    int                          mSelected;
    QColor                       mMarkerColor;
    QColor                       mSelectedMarkerColor;
  protected:

    virtual void paintEvent(QPaintEvent *event) override;
  public:
    explicit ScrollBarWithMarkers(QWidget* parent);
    void setHighlights(const  QList<QPair<double, double>>& positions, int selected);
  };






//!
//! \brief The SdWScriptEditor class Script editor widget with highlight
//!
class SdWScriptEditor : public QPlainTextEdit {
    Q_OBJECT

    //корень графа локальных блоков
    BlockNode* mRootBlock;
    //все локальные блоки для облегчения поиска
    QList<BlockNode*>     mAllBlocks;

    ScrollBarWithMarkers* mScrollBarMarker;

    int                   mAutoIndentSpacesCount;   //!< количество пробелов для автоотступа

    bool                  mLastPressedReturn;
    bool                  mAutoCompleteParenthesis;

    QListWidget          *mAutoComplete;            //!< Окно списка автозавершения
    int                   mLeftViewPortMargin;      //!< Отступ слева для номеров строк
    bool                  mIsUndoAvailable;
    bool                  mIsRedoAvailable;
    bool                  mIsSelectPresent;

    bool                  mControlPress;            //!< Флаг нажатия кнопки Control
    QTextBlock            mLinkBlock;
    QString               mOverWord;                //!< Идентификатор, над которым находится курсор

    QPlainTextEdit       *mHelpPopUp;               //!< Окно всплывающей помощи
    QTimer                mHelpTime;                //!< Таймер, обеспечивающий появление помощи к идентификатору
    QMap<QString,QString> mHelpMap;                 //!< Карта помощи по функциям


    QList<QTextEdit::ExtraSelection> mSearchHighlight; //!< подстветка поиска

    QList<QTextEdit::ExtraSelection> mParenthesisHighlight; //!< подсветка скобок

    QTextEdit::ExtraSelection        mCurrentLineHighlight; //!< подстветка текущей строки

    SdWScriptHighlighter   *mHighlighter;
  public:

    explicit SdWScriptEditor(QWidget *parent = 0);
    virtual ~SdWScriptEditor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int  lineNumberAreaWidth();

    //Получить (идентификатор) слово под курсором
    QString getWordCursorOver();
    QString getWordCursorOver( QTextCursor cursor, int *start );

    //Получить файл из include под курсором
    QString getIncludeOver();

    //Получить состояние редактора
    bool    isUndoAvailable() const { return mIsUndoAvailable; }
    bool    isRedoAvailable() const { return mIsRedoAvailable; }
    bool    isSelectPresent() const { return mIsSelectPresent; }

    int     autoIndentSpaceCount()const;
    void    setAutoIndentSpaceCount(int count);
    bool    autoCompleteParenthesis()const;
    void    setAutoCompleteParenthesis(bool autoComplete);

    void    setHighlighter( SdWScriptHighlighter *highlighter ) { mHighlighter = highlighter; }

    void    setHelpMap( const QMap<QString,QString> &map ) { mHelpMap = map; }

  private:
    //обновить подсветку
    void       updateHighlights();

    //количество пробелов в начале строки
    int        getLeadingSpaceCount(const QString &text);

    //определть в каком блоке находится строка
    BlockNode *getBlockForLine(int lineNum)const;

    //определить в каком блоке находится символ
    BlockNode *getBlockForIndex(int index)const;

    //Виджет отображающий номера строк
    QWidget  *lineNumberArea;

    //Автоматическая вставка отступов
    void      autoIndent();

    //Автоматическое завершение }
    bool      completeParenthesis();

    //Автозавершение ввода
    void      autoComplete();

    //Перезаполнить список автозавершения
    void      refillAutoComplete();

    //Получить часть слова, расположенную слева от курсора
    QString   getWordLeft();

    //Переключить комментарии
    void      toggleRemarkLine( QTextCursor tc );
    void      toggleRemark();

    //определить начало и конец блока для выделения скобок. Возвращает пару индексов символов документа <начало, конец>
    QPair<int, int> getParenthesisBlock(const QString & text, int pos)const;

    //закомментирован ли символ
    bool      isCommented(int index)const;

    void      inputDialog( QTextCursor c );
    void      selectDialog( QTextCursor c );

  private slots:
    void updateLineNumberAreaWidth(int newBlockCount);

    //подсветка текущей строки
    void highlightCurrentLine();
    //подсветка текущего блока
    void highlightCurrentBlock();

    void updateLineNumberArea(const QRect &rect, int dy);

    void onUndoAvailable( bool ena ) { mIsUndoAvailable = ena; }
    void onRedoAvailable( bool ena ) { mIsRedoAvailable = ena; }
    void onSelection( bool ena ) { mIsSelectPresent = ena; }

    //обновить граф локальных блоков
    void updateBlockGraph();

  protected:
    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void keyReleaseEvent(QKeyEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *ev) override;
    virtual void mousePressEvent(QMouseEvent *e) override;

    virtual void resizeEvent(QResizeEvent *e) override;

  signals:
    void setLink( const QString link, int start );
    void rehighlightBlock( const QTextBlock block );
    void contensChanged();
    void help( const QString &topic );

 public slots:
    void onCursorPositionChanged();
    void locateLine( int line );
    //замена текста с индекса startIndex по endIndex
    void replace(int startIndex, int endIndex, const QString & text);
    //автоотступ строк, содержащихся между позициями по символам startIndex и endIndex
    void autoIndent(int startIndex, int endIndex, bool registerUndo = true);
    //автоотступ строки
    void autoIndentLine(int lineNum, bool registerUndo = false);
    //отступ строки на заданное количество пробелов
    void indentLine(int lineNum, int spacesCount, bool registerUndo = false);
  };


class LineNumberArea : public QWidget
  {
  public:
    LineNumberArea(SdWScriptEditor *editor) : QWidget(editor) {
      codeEditor = editor;
      }

    QSize sizeHint() const Q_DECL_OVERRIDE {
      return QSize(codeEditor->lineNumberAreaWidth(), 0);
      }

  protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
      codeEditor->lineNumberAreaPaintEvent(event);
      }

  private:
    SdWScriptEditor *codeEditor;
  };

#endif // SDWSCRIPTEDITOR_H
