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




bool SdScanerVrml::parseVrml2_0()
  {
  tokenNext();
  while( !mEndOfScan && parse2Declaration( &mRootList ) );
  return true;
  }



bool SdScanerVrml::parse2Declaration(VrmlNodePtrList *list)
  {
  }

bool SdScanerVrml::parse2Group(VrmlNodePtrList *list)
  {
  Vrml
  }

