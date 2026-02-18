#ifndef SDSCANERKICAD_H
#define SDSCANERKICAD_H

#include "../SdScanerMultyline.h"

#include <QPointF>
#include <QMap>
#include <functional>

class SdProject;
class SdPItemPart;
class SdProjectItem;
class SdPoint;
class SdPvInt;
class SdPropLayer;
struct SdPropLine;

class SdScanerKiCad;

template <typename SdKiCad>
using SdKiCadTokenMap = QMap<QString,std::function<void( SdScanerKiCad*, SdKiCad* )> >;

class SdScanerKiCad : public SdScanerMultyline
  {
    double mPpm; //!< Conversion factor
  public:
    SdScanerKiCad();

    double ppm() const { return mPpm; }

    void   ppmSet( double ppm ) { mPpm = ppm; }

    int    convert( double v ) const { return v * mPpm; }

    void parseTop( SdProject *project );


    template <class SdKiCad>
    void parse( const SdKiCadTokenMap<SdKiCad> &tokenMap, SdKiCad *obj )
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

    int tokenNeedCoord() { return convert(tokenNeedValueDouble()); }

    int tokenNeedAngle() { return tokenNeedValueDouble() * 1000.0; }

    void tokenNeedClose() { tokenNeed(')'); }

    // SdScaner interface
  public:
    virtual void tokenNext() override;
  };




#endif // SDSCANERKICAD_H
