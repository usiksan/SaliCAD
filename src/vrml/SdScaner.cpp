#include "SdScaner.h"

#include <QDebug>

SdScaner::SdScaner()
  {

  }

void SdScaner::error(const QString msg)
  {
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
  if( mToken == tokenId ) {
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




bool SdScaner::matchTokenValue(const QString &val)
  {
  if( mTokenValue == val ) {
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
