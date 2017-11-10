/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Packet - is the transfer header. It describes information block.
*/
#ifndef SDCSPACKETINFO_H
#define SDCSPACKETINFO_H

#include <QString>
#include <QDataStream>
#include <QByteArray>
#include <QList>

//Commands available before login
#define SCPI_GET_SERVER_VERSION   1 //Get cad server version to decide properties
                                    //data: none
#define SCPI_SERVER_VERSION       2 //Server info packet
                                    //data: SdCadServerVersion - server version info
#define SCPI_REQUEST_AUTHOR       3 //Register new author
                                    //data: SdAuthorInfo
#define SCPI_LOGIN                4 //Login to server for author. For different authors available different possibilities
                                    //data: SdAuthorInfo
#define SCPI_ACKNOWLEDGE          5 //Acknowledge registration and login. After login available all functionality
                                    //data: SdAuthorInfo

//Commands available after login
#define SCPI_GET_UPDATE_LIST      6 //Request to get update data base list
                                    //data: quint32 - time of last upgrading
#define SCPI_UPGRADE_LIST         7 //Upgrade data base list
                                    //data: SdItemInfoList - list of upgrade obects
#define SCPI_GET_OBJECT           8 //Request to get object from data base with desired hashId
                                    //data: QString - hashId of requested object
#define SCPI_OBJECT               9 //Object
                                    //data: SdItemInfo, SdProjectItem




struct SdCadServerVersion {
    qint32  mMajor;      //Cad server major version
    qint32  mMinor;      //Cad server minor version
    QString mServerName; //Server name

    //Write to block
    void write( QByteArray &ar ) {
      ar.clear();
      QDataStream os( &ar );
      os << mMajor
         << mMinor
         << mServerName;
      }

    //Read from block
    void read( const QByteArray &ar ) {
      QDataStream is( ar );
      is >> mMajor
         >> mMinor
         >> mServerName;
      }
  };








struct SdItemInfo {
    QString mHashId;      //Unical object id
    QString mName;        //Object name
    QString mAuthor;      //Author name
    QString mTag;         //Group assotiation
    qint32  mTimeCreate;  //Time object creation
    qint32  mTimeUpgrade; //Time object reside in database
    qint64  mObjectClass; //Class of object

    SdItemInfo() : mTimeCreate(0), mTimeUpgrade(0), mObjectClass(0) {}
  };


typedef QList<SdItemInfo> SdItemInfoList;

//Serialise SdItemInfo object
inline QDataStream& operator << ( QDataStream &os, const SdItemInfo &info ) {
  os << info.mHashId
     << info.mName
     << info.mAuthor
     << info.mTimeCreate
     << info.mTimeUpgrade
     << info.mObjectClass;
  return os;
  }

//Deserialise SdItemInfo object
inline QDataStream& operator >> ( QDataStream &is, SdItemInfo &info ) {
  is >> info.mHashId
     >> info.mName
     >> info.mAuthor
     >> info.mTimeCreate
     >> info.mTimeUpgrade
     >> info.mObjectClass;
  return is;
  }





struct SdCategoryInfo {
    QString mName;
    QString mParent;
    QString mPath;
    qint32  mTime;
  };

typedef QList<SdCategoryInfo> SdCategoryInfoList;

//Serialise SdCategoryInfo object
inline QDataStream& operator << ( QDataStream &os, const SdCategoryInfo &info ) {
  os << info.mName
     << info.mParent
     << info.mPath
     << info.mTime;
  return os;
  }

//Deserialise SdCategoryInfo object
inline QDataStream& operator >> ( QDataStream &is, SdCategoryInfo &info ) {
  is >> info.mName
     >> info.mParent
     >> info.mPath
     >> info.mTime;
  return is;
  }




struct SdTranslationInfo {
    QString mName;
    QString mTranslation;
    QString mLanguage;
    qint32  mTime;
  };

typedef QList<SdTranslationInfo> SdTranslationInfoList;

//Serialise SdTranslationInfo object
inline QDataStream& operator << ( QDataStream &os, const SdTranslationInfo &info ) {
  os << info.mName
     << info.mTranslation
     << info.mLanguage
     << info.mTime;
  return os;
  }

//Deserialise SdTranslationInfo object
inline QDataStream& operator >> ( QDataStream &is, SdTranslationInfo &info ) {
  is >> info.mName
     >> info.mTranslation
     >> info.mLanguage
     >> info.mTime;
  return is;
  }



struct SdAuthorInfo {
    QString mAuthor;      //Author name
    QString mDescription; //Author description
    QString mKey;         //Author key
    qint32  mLimit;       //Limit delivery element count
  };

//Serialise SdAuthorInfo
inline QDataStream& operator << ( QDataStream &os, const SdAuthorInfo &info ) {
  os << info.mAuthor
     << info.mDescription
     << info.mKey
     << info.mLimit;
  return os;
  }

//Deserialise SdAuthorInfo
inline QDataStream& operator >> ( QDataStream &is, SdAuthorInfo &info ) {
  is >> info.mAuthor
     >> info.mDescription
     >> info.mKey
     >> info.mLimit;
  return is;
  }


class SdCsPacketInfo
  {
    quint8 mCommand;   //Command with block description
    quint8 mLenght[4]; //Block lenght
  public:
    SdCsPacketInfo( int cmd = 0, int len = 0 );

    //Command access
    void setCommand( int cmd, int len = 0 );
    int  command() const { return mCommand; }

    //Lenght access
    void setLenght( int len );
    int  lenght() const;

  };

#endif // SDCSPACKETINFO_H
