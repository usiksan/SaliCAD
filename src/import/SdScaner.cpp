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

void SdScaner::error(const QString msg)
  {
  if( mError.isEmpty() )
    mError = QStringLiteral("[%1:%2] %3").arg(lineIndex()).arg(mIndex).arg(msg);
  qDebug() << mError;
  }

bool SdScaner::tokenNeed(char tokenId, const QString errorMsg)
  {
  if( matchToken(tokenId) )
    return true;
  if( errorMsg.isEmpty() )
    error( QStringLiteral("Need token %1").arg( tokenId ) );
  else
    error( errorMsg );
  return false;
  }



QString SdScaner::tokenNeedValue( char tokenVal )
  {
  if( mToken == tokenVal ) {
    QString val = mTokenValue;
    tokenNext();
    return val;
    }
  error( QStringLiteral("Need token %1").arg(tokenVal) );
  return QString{};
  }



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




bool SdScaner::matchTokenValue( char tokenId, const QString &val)
  {
  if( mToken == tokenId && mTokenValue == val ) {
    tokenNext();
    return true;
    }
  return false;
  }



bool SdScaner::matchToken(char tokenId)
  {
  if( mToken == tokenId ) {
    tokenNext();
    return true;
    }
  return false;
  }



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
