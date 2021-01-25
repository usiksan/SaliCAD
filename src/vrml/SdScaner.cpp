#include "SdScaner.h"

SdScaner::SdScaner()
  {

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
