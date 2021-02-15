#include "SdW3dModelProgrammHighlighter.h"

//QHash<QString,QString>   Highlighter::mMacroTable;     //Таблица макроимен
//QHash<QString,int>       Highlighter::mKeyWords;       //Список ключевых слов с токенами
//QHash<QString,int>       Highlighter::mSymbols;        //Список переменных
//QHash<QString,TFileLine> Highlighter::mSymSources;     //Список ссылок на места определения


SdW3dModelProgrammHighlighter::SdW3dModelProgrammHighlighter(QTextDocument *parent)
  : QSyntaxHighlighter(parent)
  {
  //Образовать подсветку
  mKeywordFormat.setForeground(Qt::darkBlue);           //!< Ключевые слова
  mKeywordFormat.setFontWeight(QFont::Bold);

  mSingleLineCommentFormat.setForeground(Qt::darkGreen); //!< Однострочные коментарии
  mMultiLineCommentFormat.setForeground(Qt::darkGreen);  //!< Многострочный коментарий

  mQuotationFormat.setForeground(Qt::darkGreen);         //!< Строки и символы
  mQuotationFormat.setFontWeight(QFont::Bold);

  mIdentFormat.setForeground(Qt::darkMagenta);          //!< Вызов функции

  mDecimalFormat.setForeground(Qt::darkCyan);           //!< Десятичные числа

  mOctalFormat.setForeground(Qt::cyan);             //!< Восьмиричные числа

  mHexFormat.setForeground(Qt::darkYellow);               //!< Шестнадцатиричные числа

  mMacroFormat.setForeground(Qt::blue);             //!< Макрос

  mPreprocessFormat.setForeground(Qt::darkRed);        //!< Препроцессор

  mDebugFormat.setBackground( Qt::lightGray );

  mLinkFormat.setForeground( Qt::magenta );
  mLinkFormat.setFontUnderline(true);

  }





int
SdW3dModelProgrammHighlighter::MultiLineComment(const QString &text, int index, int count) {
  int startIndex = index;
  bool closed = false;
  while( index + 1 < count ) {
    if( text[index] == QChar('"') ) {
      //Пропустить строку
      index = SkipQuotation( text, index, count );
      }
    else if( text[index] == QChar('*') && text[index+1] == QChar('/') ) {
      //Коментарий закончен
      index += 2;
      closed = true;
      break;
      }
    else index++;
    }
  //установить формат для многострочного коментария
  if( closed ) {
    setFormat( startIndex, index - startIndex, mMultiLineCommentFormat );
    setCurrentBlockState( currentBlockState() | 1 );
    }
  else {
    setFormat( startIndex, count - startIndex, mMultiLineCommentFormat );
    setCurrentBlockState( currentBlockState() & ~1 );
    }
  return closed ? index : -1;
  }




void SdW3dModelProgrammHighlighter::setLink(const QString link)
  {
  mLink = link;
  }




int
SdW3dModelProgrammHighlighter::SkipQuotation(const QString &text, int index, int count) {
  index++;
  while( index < count && !(text[index] == QChar('"') && text[index - 1] != QChar('\\'))  )
    index++;
  return index < count ? index + 1 : index;
  }




void
SdW3dModelProgrammHighlighter::highlightBlock(const QString &text) {
  int index = 0;
  int count = text.count();

  //FIXME
  QRegExp expression("\".*\"");
  int quotPos = expression.indexIn(text);
  while (quotPos >= 0) {
      int length = expression.matchedLength();
      setFormat(quotPos, length, mQuotationFormat);
      quotPos = expression.indexIn(text, quotPos + length);
  }

  //Проверить не является ли текущий блок блоком отладки
  if( (currentBlockState() & 2) == 0 ) {
    setFormat( 0, count, mDebugFormat );
    return;
    }
  if( (previousBlockState() & 1) == 0 ) {
    //С прошлого блока идут многострочные коментарии
    index = MultiLineComment( text, index, count );
    //Если многострочный коментарий не завершился, то возврат
    if( index < 0 ) return;
    }
  else setCurrentBlockState( currentBlockState() | 1 );

  //Обычный разбор
  while( index < count ) {
    //Проверить двухсимвольные модификаторы
    if( index + 1 < count ) {
      if( text[index] == QChar('/') && text[index+1] == QChar('*') ) {
        //Многострочный коментарий
        index = MultiLineComment( text, index, count );
        //Если многострочный коментарий не завершился, то возврат
        if( index < 0 ) return;
        continue;
        }
      if( text[index] == QChar('#') ) {
        //Однострочный коментарий
        setFormat( index, count - index, mSingleLineCommentFormat );
        return;
        }
      }

    //Проверяем строки
    if( text[index] == QChar('"') ) {
      index = SkipQuotation( text, index, count );
      continue;
      }

    //Проверяем идентификаторы
    if( text[index].isLetter() || text[index] == QChar('_') ) {
      int startIndex = index;
      while( index < count && (text[index].isLetterOrNumber() || text[index] == QChar('_')) )
        index++;
      QString name = text.mid( startIndex, index - startIndex );

      //Выполнить поиск ключевых слов
      if( mCompiler && mCompiler->mKeyWords.contains( name ) ) {
        //Это ключевое слово
        setFormat( startIndex, index - startIndex, mKeywordFormat );
        continue;
        }

      //Выполнить поиск макросов
      if( mCompiler && mCompiler->mMacroTable.contains( name ) ) {
        //Это макрос
        setFormat( startIndex, index - startIndex, mMacroFormat );
        continue;
        }

      //Выполнить поиск идентификатора
      if( mCompiler && mCompiler->isGlobalSymbol(name) ) {
        //Это переменная
        //Проверить необходимость линковки
        if( !mLink.isEmpty() && mLink == name )
          setFormat( startIndex, index - startIndex, mLinkFormat );
        else
          setFormat( startIndex, index - startIndex, mIdentFormat );
        }
      continue;
      }

    //Проверяем числа
    if( text[index] == QChar('0') ) {
      int startIndex = index++;
      if( index < count ) {
        if( text[index] == QChar('x') ) {
          //Шестнадцатиричное число
          while( index < count && text[index].isLetterOrNumber() ) index++;
          setFormat( startIndex, index - startIndex, mHexFormat );
          continue;
          }
        //Восьмиричное число или однозначное десятичное
        while( index < count && text[index].isDigit() ) index++;
        setFormat( startIndex, index - startIndex, (index - startIndex) == 1 ? mDecimalFormat : mOctalFormat );
        continue;
        }
      else {
        setFormat( startIndex, index - startIndex, mDecimalFormat );
        continue;
        }
      }

    if( text[index].isDigit() ) {
      //Десятичное число
      int startIndex = index;
      while( index < count && text[index].isDigit() ) index++;
      setFormat( startIndex, index - startIndex, mDecimalFormat );
      continue;
      }

    //Никчемный символ
    index++;
    }
  }
