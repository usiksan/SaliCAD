#include "SdScanerKiCad.h"


SdScanerKiCad::SdScanerKiCad()
  {

  }


void SdScanerKiCad::tokenNext()
  {
  mToken = 0;
  while( !isEndOfScanOrError() ) {
    blank();
    if( !isEndOfScanOrError() ) {
      if( mLine.at(mIndex).isLetter() ) {
        mToken = 'n';
        scanName();
        }
      else if( mLine.at(mIndex).isDigit() || mLine.at(mIndex) == QChar('-') ) {
        scanDouble( true, false );
        mToken = mTokenValue.contains( QChar('.') ) ? 'f' : 'd';
        }
      else if( mLine.at(mIndex) == QChar('#') ) {
        //Remark found. Skeep line
        mIndex = mLine.length();
        continue;
        }
      else if( mLine.at(mIndex) == QChar('"') ) {
        //String detected
        mToken = 's';
        scanString( QChar('"'), QChar('\\'), QStringLiteral("Unclosed string") );
        }
      else mToken = mLine.at(mIndex++).toLatin1();
      break;
      }
    }
  }
