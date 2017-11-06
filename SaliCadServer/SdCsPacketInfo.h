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

#define SCPI_GET_SERVER_VERSION   1 //Get cad server version to decide properties
                                    //data: none
#define SCPI_SERVER_VERSION       2 //Server info packet
                                    //data: SdCadServerVersion - server version info
#define SCPI_GET_UPDATE_LIST      3 //Request to get update data base list
                                    //data: quint32 - time of last upgrading
#define SCPI_UPGRADE_LIST         4 //Upgrade data base list
                                    //data: SdItemInfoList - list of upgrade obects
#define SCPI_GET_OBJECT           5 //Request to get object from data base with desired hashId
                                    //data: QString - hashId of requested object
#define SCPI_OBJECT               6 //Object
                                    //data: SdProjectItem




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
    QString mTag0;        //Group 0 assotiation
    QString mTag1;        //Group 1 assotiation
    qint32  mObjectTime;  //Time of last object editing
    qint64  mObjectClass; //Class of object

    SdItemInfo() : mObjectTime(0), mObjectClass(0) {}
  };


typedef QList<SdItemInfo> SdItemInfoList;

//Serialise SdItemInfo object
inline QDataStream& operator << ( QDataStream &os, const SdItemInfo &info ) {
  os << info.mHashId
     << info.mName
     << info.mAuthor
     << info.mObjectTime
     << info.mObjectClass;
  return os;
  }

//Deserialise SdItemInfo object
inline QDataStream& operator >> ( QDataStream &is, SdItemInfo &info ) {
  is >> info.mHashId
     >> info.mName
     >> info.mAuthor
     >> info.mObjectTime
     >> info.mObjectClass;
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
