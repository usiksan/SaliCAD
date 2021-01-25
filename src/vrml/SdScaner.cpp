#include "SdScaner.h"

#include <QDebug>

SdScaner::SdScaner()
  {

  }

void SdScaner::error(const QString msg)
  {
  qDebug() << "[" << lineIndex() << ":" << mIndex << "]" << msg;
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




bool SdScaner::matchTokenValue(const QString &val)
  {
  if( mTokenValue == val ) {
    tokenNext();
    return true;
    }
  return false;
  }



bool SdScaner::matchToken(char token)
  {
  if( mToken == token ) {
    tokenNext();
    return true;
    }
  return false;
  }




void SdScaner::scanName()
  {
  mTokenValue.clear();
  //Digits
  while( mIndex < mLine.count() && mLine.at(mIndex).isLetterOrNumber() )
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
  while( mIndex < mLine.count() && mLine.at(mIndex).isDigit() )
    mTokenValue.append( mLine.at(mIndex++) );

  if( mIndex >= mLine.count() ) return;

  //Point
  if( mLine.at(mIndex) == QChar('.') || (useComma && mLine.at(mIndex) == QChar(',')) ) {
    mIndex++;
    mTokenValue.append( QChar('.') );
    //Digits
    while( mIndex < mLine.count() && mLine.at(mIndex).isDigit() )
      mTokenValue.append( mLine.at(mIndex++) );

    if( mIndex >= mLine.count() ) return;

    if( mLine.at(mIndex) == QChar('e') || mLine.at(mIndex) == QChar('E') ) {
      mIndex++;
      mTokenValue.append( QChar('e') );

      if( mIndex >= mLine.count() ) return;

      if( mLine.at(mIndex) == QChar('+') || mLine.at(mIndex) == QChar('-') )
        mTokenValue.append( mLine.at(mIndex++) );

      //Digits
      while( mIndex < mLine.count() && mLine.at(mIndex).isDigit() )
        mTokenValue.append( mLine.at(mIndex++) );
      }
    }
  }




void SdScaner::scanInteger(bool allowSign)
  {
  mTokenValue.clear();
  if( allowSign ) {
    if( mLine.at(mIndex) == QChar('+') || mLine.at(mIndex) == QChar('-') )
      mTokenValue.append( mLine.at(mIndex++) );
    }
  //Digits
  while( mIndex < mLine.count() && mLine.at(mIndex).isDigit() )
    mTokenValue.append( mLine.at(mIndex++) );
  }



void SdScaner::skeepBlock(char openToken, char closeToken)
  {
  if( mToken == openToken ) {
    while( mToken != closeToken ) {
      tokenNext();
      if( mEndOfScan ) {
        error( QStringLiteral("End of scan without token %1").arg(closeToken), lineIndex(), mIndex );
        return;
        }
      if( mToken == openToken )
        skeepBlock( openToken, closeToken );
      }
    tokenNext();
    }
  }
