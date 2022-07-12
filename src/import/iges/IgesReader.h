#ifndef IGESREADER_H
#define IGESREADER_H

#include "IgesDirectoryEntry.h"
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

    IgesDirectoryEntryList mDirectoryEntryList;
    QMap<int,IgesEntity*>  mDirectoryEntryMap;
    IgesParameterDataList  mParameterDataList;
    QMap<int,int>          mParameterDataMap;

    //char       m
  public:
    IgesReader();

    bool        scanFile( const QString &fname );

    bool        parse();

    IgesEntity *entity( int index ) const { return mDirectoryEntryMap.value(index); }

  private:
    bool nextLine();

    bool scanLine();

    bool scanReal( double &val, double defVal, bool thereDef, int limitIndex = 72 );
    bool scanChars( QByteArray &dest, const QByteArray &def, bool thereDef, int limitIndex = 72 );
    bool scanParametrDelimiter( int limitIndex = 72 );
    bool scanRecordDelimiter( int limitIndex = 72 );
    bool scanInt(int &val, int defVal, bool thereDef, int limitIndex = 72 );


    bool scanStart();
    bool scanGlobal();
    bool scanDirectoryEntry();
    bool scanParametrData(); //64
  };

#endif // IGESREADER_H
