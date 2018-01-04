/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Packet - is the transfer header. It describes information block.

  Interchange is:
  - receiv packet with query
  - send packet with answer
*/
#ifndef SDCSPACKETINFO_H
#define SDCSPACKETINFO_H

#include <QString>
#include <QDataStream>
#include <QByteArray>
#include <QList>

//Commands
#define SCPI_REGISTARTION_REQUEST 1 //Request to registration new user
                                    //data: SdAuthorInfo
#define SCPI_REGISTRATION_INFO    2 //Answer with registration data
                                    //data: SdCadServerVersion
                                    //      SdAuthorInfo

#define SCPI_MACHINE_REQUEST      3 //Add new machine to existing user
                                    //data: SdAuthorInfo
#define SCPI_MACHINE_INFO         4 //Answer with registration data
                                    //data: SdCadServerVersion
                                    //      SdAuthorInfo


#define SCPI_SYNC_REQUEST         5 //Request to get update data base list
                                    //data: SdAuthorInfo
                                    //      list of
                                    //        SdItemInfo, SdProjectItem
#define SCPI_SYNC_LIST            6 //Upgrade data base list
                                    //data: SdCadServerVersion
                                    //      SdAuthorInfo
                                    //      SdItemInfoList - list of upgrade obects


#define SCPI_OBJECT_REQUEST       7 //Request to get object from data base with desired hashId
                                    //data: SdAuthorInfo
                                    //      QString - hashId of requested object
#define SCPI_OBJECT               8 //Object
                                    //data: SdCadServerVersion
                                    //      SdAuthorInfo
                                    //      SdItemInfo, SdProjectItem


#define SCPE_UNDEFINED              0 //Result of operation is undefined
#define SCPE_SUCCESSFULL            1 //Successfull operation
#define SCPE_AUTHOR_IS_EMPTY        2 //Not registered or login because author is null
#define SCPE_AUTHOR_ALREADY_PRESENT 3 //Not registerd because author already registered
#define SCPE_REGISTER_FAIL          4 //Internal error when registration
#define SCPE_INVALID_KEY            5 //Invalid key when machine addon
#define SCPE_MACHINE_LIMIT          6 //Machine limit
#define SCPE_NOT_REGISTERED         7
#define SCPE_SYNC_FAIL              8
#define SCPE_OBJECT_NOT_FOUND       9
#define SCPE_OBJECT_LIMIT          10



struct SdCadServerVersion {
    qint32  mMajor;      //Cad server major version
    qint32  mMinor;      //Cad server minor version
    QString mServerName; //Server name
  };


//Serialise SdCadServerVersion object
inline QDataStream& operator << ( QDataStream &os, const SdCadServerVersion &version ) {
  os << version.mMajor
     << version.mMinor
     << version.mServerName;
  return os;
  }


//Deserialise SdCadServerVersion object
inline QDataStream& operator >> ( QDataStream &is, SdCadServerVersion &version ) {
  is >> version.mMajor
     >> version.mMinor
     >> version.mServerName;
  return is;
  }






struct SdItemInfo {
    QString mHashId;      //Unical object id
    QString mName;        //Object name
    QString mAuthor;      //Author name
    QString mTag;         //Group assotiation
    qint32  mTimeCreate;  //Time object creation
    qint64  mObjectClass; //Class of object

    SdItemInfo() : mTimeCreate(0), mObjectClass(0) {}
  };


typedef QList<SdItemInfo> SdItemInfoList;

//Serialise SdItemInfo object
inline QDataStream& operator << ( QDataStream &os, const SdItemInfo &info ) {
  os << info.mHashId
     << info.mName
     << info.mAuthor
     << info.mTimeCreate
     << info.mObjectClass;
  return os;
  }

//Deserialise SdItemInfo object
inline QDataStream& operator >> ( QDataStream &is, SdItemInfo &info ) {
  is >> info.mHashId
     >> info.mName
     >> info.mAuthor
     >> info.mTimeCreate
     >> info.mObjectClass;
  return is;
  }





struct SdCategoryInfo {
    QString mSection;
    QString mParent;
    QString mPath;
    qint32  mTime;
  };

typedef QList<SdCategoryInfo> SdCategoryInfoList;

//Serialise SdCategoryInfo object
inline QDataStream& operator << ( QDataStream &os, const SdCategoryInfo &info ) {
  os << info.mSection
     << info.mParent
     << info.mPath
     << info.mTime;
  return os;
  }

//Deserialise SdCategoryInfo object
inline QDataStream& operator >> ( QDataStream &is, SdCategoryInfo &info ) {
  is >> info.mSection
     >> info.mParent
     >> info.mPath
     >> info.mTime;
  return is;
  }




struct SdTranslationInfo {
    QString mSection;
    QString mTranslate;
    QString mLang;
    qint32  mTime;
  };

typedef QList<SdTranslationInfo> SdTranslationInfoList;

//Serialise SdTranslationInfo object
inline QDataStream& operator << ( QDataStream &os, const SdTranslationInfo &info ) {
  os << info.mSection
     << info.mTranslate
     << info.mLang
     << info.mTime;
  return os;
  }

//Deserialise SdTranslationInfo object
inline QDataStream& operator >> ( QDataStream &is, SdTranslationInfo &info ) {
  is >> info.mSection
     >> info.mTranslate
     >> info.mLang
     >> info.mTime;
  return is;
  }



struct SdAuthorInfo {
    QString mAuthor;      //Author name
    QString mDescription; //Author description
    QString mKey;         //Author key
    qint32  mLimit;       //Limit delivery element count
    qint32  mDelivered;   //Delivered element count
    quint32 mLastSync;    //Time of last syncronisation
    qint32  mResult;      //Result of operation
  };

//Serialise SdAuthorInfo
inline QDataStream& operator << ( QDataStream &os, const SdAuthorInfo &info ) {
  os << info.mAuthor
     << info.mDescription
     << info.mKey
     << info.mLimit
     << info.mDelivered
     << info.mLastSync
     << info.mResult;
  return os;
  }

//Deserialise SdAuthorInfo
inline QDataStream& operator >> ( QDataStream &is, SdAuthorInfo &info ) {
  is >> info.mAuthor
     >> info.mDescription
     >> info.mKey
     >> info.mLimit
     >> info.mDelivered
     >> info.mLastSync
     >> info.mResult;
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
