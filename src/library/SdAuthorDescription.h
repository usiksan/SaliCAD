#ifndef SDAUTHORDESCRIPTION_H
#define SDAUTHORDESCRIPTION_H

#include <QString>
#include <QDataStream>
#include <QStringList>

class SdAuthorDescription
  {
    QString mAuthorName;           //!< Author name
    QString mAuthorNameWithRank;   //!< Author name with rank and quality symbols
    // qint32  mTotalObjectCount;     //!< Total number of objects (components, cases, etc.) created by the author
    // qint32  mObjectComplaintCount; //!< Current number of objects with complaints
    // qint32  mTotalComplaintCount;  //!< Total number of complaints received for the author's objects
    quint8  mFlags;                //!< Reserved
    quint8  mRank;                 //!< Author volume rank
    quint8  mQuality;              //!< Author quality rating
  public:
    enum SdAuthorVolume {
      SdAv10 = 0,
      SdAv50,
      SdAv200,
      SdAv1000,
      SdAvLegend
    };

    enum SdAuthorQuality {
      SdAqHigh = 0,
      SdAqNovice,
      SdAqGood,
      SdAqMedium,
      SdAqLow
    };



    //!
    //! \brief SdAuthorDescription Default constructor
    //!
    SdAuthorDescription() : mFlags(0), mRank(SdAv10), mQuality(SdAqNovice) {}



    //!
    //! \brief authorName          Get author name
    //! \return                    Author name string
    //!
    QString authorName() const { return mAuthorName; }



    //!
    //! \brief authorRank          Get author volume rank
    //! \return                    Author volume rank value
    //!
    int     authorRank() const { return mRank; }


    //!
    //! \brief authorQuality       Get author quality rating
    //! \return                    Author quality rating value
    //!
    int     authorQuality() const { return mQuality; }


    //!
    //! \brief authorNameWithRank  Get author name with rank and quality symbols
    //! \return                    Formatted author name with symbols
    //!
    QString authorNameWithRank() const { return mAuthorNameWithRank; }



    //!
    //! \brief update              Update author description with new statistics
    //! \param authorName          New author name
    //! \param totalObjectCount    Total number of objects created
    //! \param objectCompliantCount Current number of objects with complaints
    //! \param totalCompliantCount  Total number of complaints in history
    //!
    void update( const QString &authorName, int totalObjectCount, int objectCompliantCount, int totalCompliantCount )
      {
      static QStringList rankList( {"○","◔","◐","◕","●"} );
      static QStringList qualityList( {"✔","□","■","⚠","✗"} );

      mAuthorName = authorName;
      mRank       = calcAuthorRank( totalObjectCount );
      mQuality    = calcAuthorQuality( totalObjectCount, objectCompliantCount, totalCompliantCount );

      mAuthorNameWithRank = mAuthorName + QString("[%1%2]").arg( rankList.at(mRank), qualityList.at(mQuality) );
      }

  private:

    //!
    //! \brief calcAuthorRank      Calculate author volume rank based on total object count
    //! \param totalObjectCount    Total number of objects created
    //! \return                    Author volume rank
    //!
    SdAuthorVolume calcAuthorRank( int totalObjectCount ) const
      {
      if( totalObjectCount < 10 ) return SdAv10;
      if( totalObjectCount < 50 ) return SdAv50;
      if( totalObjectCount < 200 ) return SdAv200;
      if( totalObjectCount < 1000 ) return SdAv1000;
      return SdAvLegend;
      }



    //!
    //! \brief calculateQualityScore Calculate quality score based on complaint statistics
    //! \param totalObjectCount      Total number of objects created
    //! \param totalCompliantCount   Total number of complaints in history
    //! \return                      Quality score in range 0.0 to 1.0
    //!
    double calculateQualityScore( double totalObjectCount, double totalCompliantCount ) const
      {
      if( totalObjectCount == 0 ) return 1.0; // For authors without objects

      // Basic formula: penalize for each complaint in history
      double baseScore = 1.0 - totalCompliantCount / totalObjectCount;

      // Increase penalty for authors with small number of objects
      // Fewer objects means each complaint affects rating more strongly
      if( totalObjectCount < 20 ) {
        double trustFactor = totalObjectCount / 20.0; // from 0.05 to 1.0
        baseScore = baseScore * trustFactor + (1.0 - trustFactor) * 0.5;
        }

      // Protection against negative values and range overflow
      return qBound( 0.0, baseScore, 1.0 );
      }



    //!
    //! \brief calcAuthorQuality    Calculate author quality rating
    //! \param totalObjectCount     Total number of objects created
    //! \param objectCompliantCount Current number of objects with complaints
    //! \param totalCompliantCount  Total number of complaints in history
    //! \return                     Author quality rating
    //!
    SdAuthorQuality calcAuthorQuality( double totalObjectCount, double objectCompliantCount, double totalCompliantCount ) const {
      // Special case: beginners (<10 objects)
      if( totalObjectCount < 10 ) {
        if( objectCompliantCount <= 0 )
          return SdAqNovice; // For beginner this is □
        else if( objectCompliantCount <= 2 )
          return SdAqMedium; // ⚠
        else
          return SdAqLow; // ✗
        }

      // For experienced authors (>=10 objects)
      double qualityScore = calculateQualityScore( totalObjectCount, totalCompliantCount );

      // Consider current active complaints as additional penalty
      double currentPenalty = objectCompliantCount / totalObjectCount * 0.3;
      double finalScore = qualityScore - currentPenalty;

      // Threshold values for categories
      if( finalScore >= 0.95 ) {
        // Less than 5% complaints from total object count AND 0 active complaints
        if( totalCompliantCount / totalObjectCount <= 0.05 && objectCompliantCount <= 0 )
          return SdAqHigh; // ✔
        return SdAqGood; // ■
        }
      else if( finalScore >= 0.80 )
        return SdAqGood; // ■
      else if( finalScore >= 0.60 )
        return SdAqMedium; // ⚠

      return SdAqLow; // ✗
      }

    friend QDataStream &operator << ( QDataStream &os, const SdAuthorDescription &ref );
    friend QDataStream &operator >> ( QDataStream &is, SdAuthorDescription &ref );
  };

//Write function
inline QDataStream &operator << ( QDataStream &os, const SdAuthorDescription &ref )
  {
  os << ref.mAuthorName << ref.mFlags << ref.mRank << ref.mQuality;
  return os;
  }


//Read function
inline QDataStream &operator >> ( QDataStream &is, SdAuthorDescription &ref )
  {
  is >> ref.mAuthorName >> ref.mFlags >> ref.mRank >> ref.mQuality;
  return is;
  }

#endif // SDAUTHORDESCRIPTION_H
