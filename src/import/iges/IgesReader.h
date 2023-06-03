#ifndef IGESREADER_H
#define IGESREADER_H

#include "IgesEntity.h"
#include "IgesParameterData.h"

#include <QString>
#include <QFile>
#include <QMap>

class IgesReader
  {
    QFile                  mFile;                 //!< IGES file
    QByteArray             mLine;                 //!< Working line (when scan and when parsing)
    int                    mIndex;                //!< Current char index in working line
    int                    mLineIndex;            //!< Current line index
    char                   mParametrDelimiter;    //!< Parameter delimiter
    char                   mRecordDelimiter;      //!< Record delimiter
    double                 mScale;                //!< Scale factor
    double                 mUnitFactor;           //!< Unit factor
    int                    mLineWeightGradations;
    double                 mLineMaxWeight;
    double                 mMinimumResolution;
    double                 mMaximumCoordinate;

    QMap<int,IgesEntity*>  mDirectoryEntryMap;    //!< Directory entity map. Mapping entity index (int) to entity
    IgesParameterDataMap   mParameterDataMap;     //!< Parameter data map. Mapping data block index (int) to parameter data block
  public:
    IgesReader();

    //!
    //! \brief scanFile Scan IGES file and divide it into 4 part for subsequent parsing
    //! \param fname    File name for scan
    //! \return         true when scan success
    //!
    bool        scanFile( const QString &fname );

    //!
    //! \brief parse Parse all IGES entity into internal representation
    //! \return      true when parsing success
    //!
    bool        parse();

    //!
    //! \brief generateFaces Generate faces into model
    //! \param model         Model into which will be generated
    //!
    void        generateFaces( Sd3drModel *model );

    //!
    //! \brief entity Retrive entity by index
    //! \param index  Index of entity
    //! \return       Entity
    //!
    IgesEntity *entity( int index ) const { return mDirectoryEntryMap.value(index); }

    template <class SdEntity>
    SdEntity   *entityEx( int index ) const { return dynamic_cast<SdEntity*>( entity(index) ); }

    //!
    //! \brief parameterData Retrive parameter data block by index
    //! \param index         Index of parameter data block
    //! \return              Parameter data block
    //!
    QByteArray  parameterData( int index ) const { return mParameterDataMap.value(index); }

    //!
    //! \brief parameterDataInit Init parameter scaner by index of parameter data block
    //! \param index             Index of parameter data block
    //!
    void        parameterDataInit( int index ) { setLine( parameterData(index) ); }

    //!
    //! \brief setLine Init parameter scaner
    //! \param line    Parameter block
    //!
    void        setLine( const QByteArray &line );

    //!
    //! \brief paramInt Decode next value as int
    //! \param val      Int value
    //! \return         true if decoding success
    //!
    bool        paramInt( int &val );

    //!
    //! \brief paramReal Decode next value as double
    //! \param val       Double value
    //! \return          true if decoding success
    //!
    bool        paramReal( double &val );

    bool        paramEntityPtr( IgesEntityPtr &entityPtr );

//    template <class SdEntityPtr>
//    bool        paramEntityExPtr( SdEntityPtr &entityPtr )
//      {
//      IgesEntityPtr ep;
//      bool res = paramEntityPtr( ep );
//      entityPtr = dynamic_cast<SdEntityPtr>( ep );
//      return res && (ep == nullptr || entityPtr == nullptr);
//      }

  private:
    //!
    //! \brief nextLine Retrive next line from file
    //! \return         true if next line readed successfull
    //!
    bool nextLine();

    //!
    //! \brief scanLine Perform scan current line of file
    //! \return         true of scaning successfull
    //!
    bool scanLine();

    //!
    //! \brief scanStart Scan start part of file
    //! \return          true of scaning successfull
    //!
    bool scanStart();

    //!
    //! \brief scanGlobal Scan global part of file
    //! \return           true of scaning successfull
    //!
    bool scanGlobal();

    //!
    //! \brief scanDirectoryEntry Scan single entry of directory of directory part of file
    //! \return                   true if scaning successfull
    //!
    bool scanDirectoryEntry();

    //!
    //! \brief scanParametrData Scan single parameter block of parameter part of file
    //! \return                 true if scaning successfull
    //!
    bool scanParametrData(); //64

    //!
    //! \brief scanReal Scan double value from current position of work line (mLine)
    //! \param val      Scanned value
    //! \param defVal   Default value will assign to scanned value if field is empty (optional)
    //! \param thereDef If true then value may be absent and default value assigned, other side - value must be present
    //! \return         true if scaning successfull
    //!
    bool scanReal( double &val, double defVal, bool thereDef );

    //!
    //! \brief scanChars Scan chars string from current position of work line (mLine)
    //! \param dest      Scanned string
    //! \param defVal    Default value will assign to scanned value if field is empty (optional)
    //! \param thereDef  If true then value may be absent and default value assigned, other side - value must be present
    //! \return          true if scaning successfull
    //!
    bool scanChars( QByteArray &dest, const QByteArray &def, bool thereDef );

    //!
    //! \brief scanParametrDelimiter Scan parameter delimiter
    //! \return                      true if scaning successfull
    //!
    bool scanParametrDelimiter();

    //!
    //! \brief scanRecordDelimiter Scan record delimiter
    //! \return                    true if scaning successfull
    //!
    bool scanRecordDelimiter();

    //!
    //! \brief scanInt  Scan int value from current position of work line (mLine)
    //! \param val      Scanned value
    //! \param defVal   Default value will assign to scanned value if field is empty (optional)
    //! \param thereDef If true then value may be absent and default value assigned, other side - value must be present
    //! \return         true if scaning successfull
    //!
    bool scanInt(int &val, int defVal, bool thereDef );

  };

#endif // IGESREADER_H
