#include "SdScriptScaner.h"

SdScriptScaner::SdScriptScaner()
  {

  }


void SdScriptScaner::tokenNext()
  {
  mToken = 0;
  while( !isEndOfScanOrError() ) {
    blank();
    if( !isEndOfScanOrError() ) {
      if( mLine.at(mIndex).isLetter() ) {
        mToken = 'n';
        scanName();
        }
      else if( mLine.at(mIndex).isDigit() ) {
        scanDouble( true, false );
        mToken = 'd';
        }
      else if( mLine.at(mIndex) == QChar('#') ) {
        //Remark found. Skeep line
        mIndex = mLine.count();
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
