/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdScaner - base scaner for text import

  Predefined tokens:
    'f' - for floats
    'd' - for integer
  Ordinary tokens (but may be others):
    'n' - for names
    's' - for strings
    Special chars like '(' represents itself.
*/
#include "SdScaner.h"

#include <QDebug>

SdScaner::SdScaner() :
  mToken(0),         //Current scanned token
  mLine(),           //Input line
  mIndex(0),         //Current char index
  mEndOfScan(true),  //Flag of scan completion
  mError(),          //Error string
  mTokenValue()      //Value of current token
  {

  }




//!
//! \brief error Append error string to error system
//! \param msg   Error message
//!
void SdScaner::error(const QString msg)
  {
  if( mError.isEmpty() )
    mError = QStringLiteral("[%1:%2] %3").arg(lineIndex()).arg(mIndex).arg(msg);
  qDebug() << mError;
  }





//!
//! \brief tokenNeed Check if last scanned token equal to tokenId and if not then append error with errorMsg
//! \param tokenId   Needed token ident
//! \param errorMsg  Error message which appended if tokenId not equal to last scanned token
//! \return          true when tokenId equals to last scanned token
//!
bool SdScaner::tokenNeed(char tokenId, const QString errorMsg)
  {
  //Compare token
  if( matchToken(tokenId) )
    return true;

  //Token not equal
  //We append error message. If errorMsg is empty then we build its itself
  if( errorMsg.isEmpty() )
    error( QStringLiteral("Need token %1").arg( tokenId ) );
  else
    error( errorMsg );
  return false;
  }




//!
//! \brief tokenNeedValue Check if last scanned token equals to tokenVal and return value itself
//! \param tokenVal       Needed token ident
//! \return               Value of token if last scanned token equals to tokenVal
//!
QString SdScaner::tokenNeedValue( char tokenVal )
  {
  if( (tokenVal == 'f' && mToken == 'd') || (mToken == tokenVal) ) {
    QString val = mTokenValue;
    tokenNext();
    return val;
    }
  error( QStringLiteral("Need token %1").arg(tokenVal) );
  return QString{};
  }





//!
//! \brief tokenNeedValue Check if last scanned token equals to tokenVal and return value itself
//! \param tokenId        Needed token ident
//! \param value          Value of token if last scanned token equals to tokenId
//! \param errorMsg       Error message which appended to error list if last scanned token not equals to tokenId
//! \return               true when tokenId equals to last scanned token
//!
bool SdScaner::tokenNeedValue(char tokenId, QString &value, const QString errorMsg)
  {
  if( mToken == 0 )
    return false;
  //Append special case when integer need to be act as float
  if( (tokenId == 'f' && mToken == 'd') || (mToken == tokenId) ) {
    value = mTokenValue;
    tokenNext();
    return true;
    }
  error( errorMsg );
  return false;
  }





//!
//! \brief tokenNeedValueInt  Check if last scanned token equals to tokenId and return value itself as integer
//! \param tokenId            Needed token ident
//! \param value              Value of token if last scanned token equals to tokenId
//! \param errorMsg           Error message which appended to error list if last scanned token not equals to tokenId
//! \return                   true when tokenId equals to last scanned token and no error in conversion to integer
//!
bool SdScaner::tokenNeedValueInt(char tokenId, int &value, const QString errorMsg)
  {
  QString strVal;
  if( tokenNeedValue( tokenId, strVal, errorMsg ) ) {
    bool res;
    value = strVal.toInt( &res );
    return res;
    }
  return false;
  }





//!
//! \brief tokenNeedValueFloat  Check if last scanned token equals to tokenId and return value itself as float
//! \param tokenId              Needed token ident
//! \param value                Value of token if last scanned token equals to tokenId
//! \param errorMsg             Error message which appended to error list if last scanned token not equals to tokenId
//! \return                     true when tokenId equals to last scanned token and no error in conversion to float
//!
bool SdScaner::tokenNeedValueFloat(char tokenId, float &value, const QString errorMsg)
  {
  QString strVal;
  if( tokenNeedValue( tokenId, strVal, errorMsg ) ) {
    bool res;
    value = strVal.toFloat( &res );
    return res;
    }
  return false;
  }






//!
//! \brief matchTokenValue Checks if last scanned token equals to tokenId and token value equals to val. If both are true then take next token
//! \param tokenId         Needed token ident
//! \param val             Value of token to compare
//! \return                true when last scanned token equals to tokenId and token value equals to val.
//!
bool SdScaner::matchTokenValue( char tokenId, const QString &val)
  {
  if( mToken == tokenId && mTokenValue == val ) {
    tokenNext();
    return true;
    }
  return false;
  }





//!
//! \brief matchToken Compares last scanned token to tokenId and if equals then take next token
//! \param tokenId    Token ident to compare
//! \return           true if last scanned token equals to tokenId
//!
bool SdScaner::matchToken(char tokenId)
  {
  if( mToken == tokenId ) {
    tokenNext();
    return true;
    }
  return false;
  }




//!
//! \brief lineSet Sets new line to scan
//! \param line    New line to scan
//!
void SdScaner::lineSet(const QString &line)
  {
  mLine = line;
  mIndex = 0;
  mEndOfScan = false;
  }




//!
//! \brief blank Skeeps all characters act as spaces
//!
void SdScaner::blank()
  {
  while( true ) {
    if( mIndex >= mLine.length() ) {
      //Current string is ended, take next line
      if( !nextLine() ) {
        mEndOfScan = true;
        return;
        }
      mIndex = 0;
      continue;
      }
    if( mLine.at(mIndex).isSpace() )
      mIndex++;
    else return;
    }
  }




//!
//! \brief scanName Scan name. Name consists of letters, digits and _ sign
//!
void SdScaner::scanName()
  {
  mTokenValue.clear();
  //Letters or numbers
  while( mIndex < mLine.length() && (mLine.at(mIndex).isLetterOrNumber() || mLine.at(mIndex) == QChar('_')) )
    mTokenValue.append( mLine.at(mIndex++) );
  }





//!
//! \brief scanDouble Scan double digit in common format -123.345e-24
//! \param allowSign  Allow scan trailing sign
//! \param useComma   Use comma as delimiter -123,345e-24
//!
void SdScaner::scanDouble(bool allowSign, bool useComma)
  {
  mTokenValue.clear();
  if( allowSign ) {
    if( mLine.at(mIndex) == QChar('+') || mLine.at(mIndex) == QChar('-') )
      mTokenValue.append( mLine.at(mIndex++) );
    }
  //Digits
  while( mIndex < mLine.length() && mLine.at(mIndex).isDigit() )
    mTokenValue.append( mLine.at(mIndex++) );

  if( mIndex >= mLine.length() ) return;

  //Point
  if( mLine.at(mIndex) == QChar('.') || (useComma && mLine.at(mIndex) == QChar(',')) ) {
    mIndex++;
    mTokenValue.append( QChar('.') );
    //Digits
    while( mIndex < mLine.length() && mLine.at(mIndex).isDigit() )
      mTokenValue.append( mLine.at(mIndex++) );

    if( mIndex >= mLine.length() ) return;

    if( mLine.at(mIndex) == QChar('e') || mLine.at(mIndex) == QChar('E') ) {
      mIndex++;
      mTokenValue.append( QChar('e') );

      if( mIndex >= mLine.length() ) return;

      if( mLine.at(mIndex) == QChar('+') || mLine.at(mIndex) == QChar('-') )
        mTokenValue.append( mLine.at(mIndex++) );

      //Digits
      while( mIndex < mLine.length() && mLine.at(mIndex).isDigit() )
        mTokenValue.append( mLine.at(mIndex++) );
      }
    }
  }




//!
//! \brief scanInteger Scan integer digit in format -1234
//! \param allowSign   Allow scan trailing sign
//!
void SdScaner::scanInteger(bool allowSign)
  {
  mTokenValue.clear();
  if( allowSign ) {
    if( mLine.at(mIndex) == QChar('+') || mLine.at(mIndex) == QChar('-') )
      mTokenValue.append( mLine.at(mIndex++) );
    }
  //Digits
  while( mIndex < mLine.length() && mLine.at(mIndex).isDigit() )
    mTokenValue.append( mLine.at(mIndex++) );
  }




//!
//! \brief scanString          Scan string closed with closeChar
//! \param closeChar           Character used to close string
//! \param guardChar           Character used as special guarding (\n)
//! \param unclosedStringError Error line when string not closed to end of line
//!
void SdScaner::scanString(QChar closeChar, QChar guardChar, const QString unclosedStringError)
  {
  mTokenValue.clear();
  //Skeep starting open symbol
  mIndex++;
  //String chars
  while( mIndex < mLine.length() ) {
    if( mLine.at(mIndex) == closeChar ) {
      //String closed
      mIndex++;
      return;
      }
    if( mLine.at(mIndex) == guardChar ) {
      //Guarding symbol
      mIndex++;
      if( mIndex >= mLine.length() ) {
        error( unclosedStringError );
        return;
        }
      }
    mTokenValue.append( mLine.at(mIndex++) );
    }
  error( unclosedStringError );
  }




//!
//! \brief skeepBlock Skeep block bounded with openToken and closeToken. Block may be nested
//! \param openToken  Mark start of block
//! \param closeToken Mark end of block
//!
void SdScaner::skeepBlock(char openToken, char closeToken)
  {
  if( mToken == openToken ) {
    while( mToken != closeToken ) {
      tokenNext();
      if( mEndOfScan ) {
        error( QStringLiteral("End of scan without token %1").arg(closeToken) );
        return;
        }
      if( mToken == openToken )
        skeepBlock( openToken, closeToken );
      }
    tokenNext();
    }
  }
