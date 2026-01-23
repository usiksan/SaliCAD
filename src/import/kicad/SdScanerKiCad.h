#ifndef SDSCANERKICAD_H
#define SDSCANERKICAD_H

#include "../SdScanerMultyline.h"

#include <QMap>
#include <functional>

class SdProject;
class SdPItemPart;
class SdProjectItem;
class SdPoint;
class SdPropInt;
class SdPropLayer;
class SdPropLine;

class SdScanerKiCad : public SdScanerMultyline
  {
  public:
    SdScanerKiCad();

    void parseTop( SdProject *project );


    template <class SdKiCad>
    void parse( const QMap<QString,std::function<void( SdScanerKiCad*, SdKiCad* )> > &tokenMap, SdKiCad *obj )
      {
      while( !matchToken(')') && !isError() )
        parseContent( tokenMap, obj );
      }



    template <class SdKiCad>
    void parseContent( const QMap<QString,std::function<void( SdScanerKiCad*, SdKiCad* )> > &tokenMap, SdKiCad *obj )
      {
      QString tokenName;
      if( tokenNeed( '(' ) && tokenNeedValue( 'n', tokenName, QObject::tr("Need token name") ) ) {
        if( tokenMap.contains(tokenName) )
          tokenMap.value(tokenName)( this, obj );
        else {
          //Skeep undefined token
          int level = 0;
          while( token() != ')' || level != 0 ) {
            if( token() == '(' ) level++;
            if( token() == ')' ) level--;
            tokenNext();
            if( isEndOfScanOrError() ) return;
            }
          tokenNext();
          }
        }
      }

    int tokenNeedPartCoord() { return tokenNeedValueFloat('f') * 1000.0; }

    // SdScaner interface
  public:
    virtual void tokenNext() override;
  };


void kicadFootprint( SdScanerKiCad *scaner, SdProject *project );
void kicadSymbolLib( SdScanerKiCad *scaner, SdProject *project );
void kicadPropertyPart( SdScanerKiCad *scaner, SdPItemPart *part );
void kicadLinePart( SdScanerKiCad *scaner, SdPItemPart *part );
void kicadCirclePart( SdScanerKiCad *scaner, SdPItemPart *part );
void kicadTextPart( SdScanerKiCad *scaner, SdPItemPart *part );
void kicadPoint( SdScanerKiCad *scaner, SdPoint &point );
void kicadLineType( SdScanerKiCad *scaner, SdPropInt &type );
void kicadLineWidth( SdScanerKiCad *scaner, SdPropInt &width );
void kicadLayer( SdScanerKiCad *scaner, SdPropLayer &layer );
void kicadStroke( SdScanerKiCad *scaner, SdPropLine *prop );

#endif // SDSCANERKICAD_H
