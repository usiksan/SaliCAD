#include "SdScanerVrml.h"

SdScanerVrml::SdScanerVrml()
  {

  }




SdScanerVrml::~SdScanerVrml()
  {
  qDeleteAll(mRootList);
  }




void SdScanerVrml::clear()
  {
  qDeleteAll(mRootList);
  mRootList.clear();
  mRootMap.clear();
  }

bool SdScanerVrml::parseFile(const QString &path)
  {
  clear();
  if( !readFile(path) )
    return false;

  if( nextLine() ) {
    if( line().startsWith( QStringLiteral("#VRML V2.0 utf8")) ) {
      nextLine();
      return parseVrml2_0();
      }
    else if( line().startsWith( QStringLiteral("#VRML V1.0 ascii")) ) {
      nextLine();
      return parseVrml1_0();
      }
    }

  return false;
  }



bool SdScanerVrml::tokenNeedValueBool(bool &value, const QString errorMsg)
  {
  QString strVal;
  if( tokenNeedValue( 'n', strVal, errorMsg ) ) {
    value = strVal == QStringLiteral("TRUE");
    return true;
    }
  return false;
  }




bool SdScanerVrml::parseInt32Table(VrmlInt32List &table, const QString errorMsg)
  {
  table.clear();
  if( matchToken('[') ) {
    while( !matchToken(']') ) {
      int val;
      if( !tokenNeedValueInt( 'd', val, QStringLiteral("Need int value") ) )
        return false;
      table.append( val );
      if( !matchToken(',') ) {
        if( !tokenNeed(']', QStringLiteral("Need ]") ) )
          return false;
        break;
        }
      }
    return true;
    }
  error( errorMsg );
  return false;
  }

bool SdScanerVrml::parseVectorTable(VrmlVectorList &table, const QString errorMsg)
  {
  if( !tokenNeed( '[', errorMsg ) )
    return false;
  while( !matchToken( ']') ) {
    if( isEndOfScan() ) {
      error( QStringLiteral("Uncompleted vector list") );
      return false;
      }
    if( isError() )
      return false;
    VrmlVector vec;
    vec.parse( this );
    table.append( vec );
    if( !matchToken(',') ) {
      if( !tokenNeed(']', QStringLiteral("Need ]") ) )
        return false;
      break;
      }
    }
  return true;
  }




bool SdScanerVrml::parseVrml2_0()
  {
  tokenNext();
  while( !isEndOfScanOrError() ) {
    VrmlNode *node = VrmlNode::parse2Declaration( this );
    if( node != nullptr )
      mRootList.append( node );
    }
  return !isError();
  }

bool SdScanerVrml::parseVrml1_0()
  {
  return false;
  }






void SdScanerVrml::tokenNext()
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
