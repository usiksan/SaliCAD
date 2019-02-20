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
#define SCPI_REGISTARTION_REQUEST  1 //Request to registration new user
                                     //data: SdAuthorInfo
#define SCPI_REGISTRATION_INFO     2 //Answer with registration data
                                     //data: SdCadServerVersion
                                     //      SdAuthorInfo

#define SCPI_MACHINE_REQUEST       3 //Add new machine to existing user
                                     //data: SdAuthorInfo
#define SCPI_MACHINE_INFO          4 //Answer with registration data
                                     //data: SdCadServerVersion
                                     //      SdAuthorInfo


#define SCPI_SYNC_REQUEST          5 //Request to get update data base list
                                     //data: SdAuthorInfo
                                     //      list of
                                     //        SdItemInfo, SdProjectItem
#define SCPI_SYNC_LIST             6 //Upgrade data base list
                                     //data: SdCadServerVersion
                                     //      SdAuthorInfo
                                     //      SdItemInfoList - list of upgrade obects


#define SCPI_OBJECT_REQUEST        7 //Request to get object from data base with desired hashId
                                     //data: SdAuthorInfo
                                     //      QString - hashId of requested object
#define SCPI_OBJECT                8 //Object
                                     //data: SdCadServerVersion
                                     //      SdAuthorInfo
                                     //      SdItemInfo, SdProjectItem

#define SCPI_FILE_REQUEST          9 //Request to get file from data base with desired fileName
                                     //data: SdAuthorInfo
                                     //      QString - fileName of requested file
#define SCPI_FILE                 10 //File
                                     //data: SdCadServerVersion
                                     //      SdAuthorInfo
                                     //      contents of file

#define SCPI_ACCESS_CHECK_REQUEST 11 //Request to check if registered.
                                     //data: SdAuthorInfo
#define SCPI_ACCESS_CHECK_ACK     12 //Acknowledges with registration data
                                     //data: SdCadServerVersion
                                     //      SdAuthorInfo

#define SCPI_DELETE_REQUEST       13 //Request to remove object from data base
                                     //data: SdAuthorInfo
                                     //      QString - hashId of requested object
#define SCPI_DELETE_ACK           14 //Object deleting acknowledge
                                     //data: SdCadServerVersion
                                     //      SdAuthorInfo


#define SCPE_UNDEFINED              0 //Result of operation is undefined
#define SCPE_SUCCESSFULL            1 //Successfull operation
#define SCPE_AUTHOR_IS_EMPTY        2 //Not registered or login because author is null
#define SCPE_AUTHOR_ALREADY_PRESENT 3 //Not registerd because author already registered
//#define SCPE_REGISTER_FAIL          4 //Internal error when registration
//#define SCPE_INVALID_KEY            5 //Invalid key when machine addon
#define SCPE_MACHINE_LIMIT          6 //Machine limit
#define SCPE_NOT_REGISTERED         7
//#define SCPE_SYNC_FAIL              8
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










struct SdAuthorInfo {
    QString mAuthor;      //Author name
    QString mEmail;
    quint64 mKey;         //Author key
    qint32  mRemain;      //Remain object count for loading
    qint32  mSyncIndex;   //Index of sync

    SdAuthorInfo() : mAuthor(), mEmail(), mKey(0), mRemain(0), mSyncIndex(0) {}
    SdAuthorInfo( const QString author, quint64 key, qint32 syncIndex ) : mAuthor(author), mEmail(), mKey(key), mRemain(0), mSyncIndex(syncIndex) {}

    //Set result of query
    void setResult( int res ) { mSyncIndex = res; }

    //Set fail query with result
    void setFail( int res ) { mAuthor.clear(); mEmail.clear(); mKey = 0; mRemain = 0; setResult(res); }

    //Test login fail
    bool isLoginFail() const { return mKey == 0 || mAuthor.isEmpty(); }

    int  result() const { return mSyncIndex; }

    bool isSuccessfull() const { return result() == SCPE_SUCCESSFULL; }
  };

//Serialise SdAuthorInfo
inline QDataStream& operator << ( QDataStream &os, const SdAuthorInfo &info ) {
  os << info.mAuthor
     << info.mEmail
     << info.mKey
     << info.mRemain
     << info.mSyncIndex;
  return os;
  }

//Deserialise SdAuthorInfo
inline QDataStream& operator >> ( QDataStream &is, SdAuthorInfo &info ) {
  is >> info.mAuthor
     >> info.mEmail
     >> info.mKey
     >> info.mRemain
     >> info.mSyncIndex;
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
