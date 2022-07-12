#ifndef IGESREADER_H
#define IGESREADER_H

#include "IgesEntity.h"
#include "IgesParameterData.h"

#include <QString>
#include <QFile>
#include <QMap>

class IgesReader
  {
    QFile      mFile;
    QByteArray mLine;
    int        mIndex;
    int        mLineIndex;
    char       mParametrDelimiter;
    char       mRecordDelimiter;
    double     mScale;
    double     mUnitFactor;
    int        mLineWeightGradations;
    double     mLineMaxWeight;
    double     mMinimumResolution;
    double     mMaximumCoordinate;

    QMap<int,IgesEntity*>  mDirectoryEntryMap;
    IgesParameterDataMap   mParameterDataMap;

    //char       m
  public:
    IgesReader();

    bool        scanFile( const QString &fname );

    bool        parse();

    IgesEntity *entity( int index ) const { return mDirectoryEntryMap.value(index); }

    QByteArray  parameterData( int index ) const { return mParameterDataMap.value(index); }

    void        parameterDataInit( int index ) { setLine( parameterData(index) ); }

    void        setLine( const QByteArray &line );

    bool        paramInt( int &val );

    bool        paramReal( double &val );

  private:
    bool nextLine();

    bool scanLine();

    bool scanReal( double &val, double defVal, bool thereDef );
    bool scanChars( QByteArray &dest, const QByteArray &def, bool thereDef );
    bool scanParametrDelimiter();
    bool scanRecordDelimiter();
    bool scanInt(int &val, int defVal, bool thereDef );


    bool scanStart();
    bool scanGlobal();
    bool scanDirectoryEntry();
    bool scanParametrData(); //64
  };

#endif // IGESREADER_H
