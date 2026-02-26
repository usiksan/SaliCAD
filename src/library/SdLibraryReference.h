/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  All variable object saved in library. It are cad objects, translate strings, hierarhy system and so on
  Every object have its unical id (hash). With its hash we store reference to header - small object portion and
  to object - main object body.

  All information saved in three files: references file, headers file and objects file. References file contains
  map of all known objects with reference to its header and object.
*/
#ifndef SDLIBRARYREFERENCE_H
#define SDLIBRARYREFERENCE_H

#include "objects/SdFileUid.h"

#include <QDataStream>


struct SdLibraryReference
  {
    qint32 mCreationTime;          //!< Time of object creation
    qint32 mInsertionTimePrivate;  //!< Time of private object insertion into library
    qint32 mInsertionTimePublic;   //!< Time of public object insertion into library
    qint64 mHeaderPtr;             //!< Header reference. It contains offset from begining headers file to header of this object
    qint32 mComplaintCount;        //!< Count of complaint about the object
    bool   mIsLocalChanged;

    SdLibraryReference() : mCreationTime(0), mInsertionTimePrivate(0), mInsertionTimePublic(0), mHeaderPtr(0), mComplaintCount(0) {}

    bool isRemoved() const { return mHeaderPtr == 0; }

    void markAsRemoved( qint32 insertionTime )
      {
      if( isPublic() ) mInsertionTimePublic = insertionTime;
      else             mInsertionTimePrivate = insertionTime;
      mHeaderPtr = 0;
      }

    bool isInsertAfterPrivate( qint32 time ) const { return mInsertionTimePrivate > time; }

    bool isInsertAfterPublic( qint32 time ) const { return mInsertionTimePublic > time; }

    //Test if reference newer than time
    bool isNewer( qint32 time ) const { return mCreationTime > time; }

    //Test if reference newer or same than time
    bool isNewerOrSame( qint32 time ) const { return mCreationTime >= time; }

    bool isOlder( qint32 time ) const { return mCreationTime < time; }

    bool isPublic() const { return mInsertionTimePublic >= mInsertionTimePrivate; }

    bool isLocalChanged() const { return mIsLocalChanged; }

    bool isRemovePrivate() const { return mInsertionTimePrivate <= mInsertionTimePublic || mHeaderPtr == 0; }

    bool isRemovePublic() const { return mInsertionTimePublic < mInsertionTimePrivate || mHeaderPtr == 0; }

    QString fileName( const QString &hashUidName ) const { return SdFileUid::fileUid( hashUidName, mCreationTime ); }
  };


//Write function
inline QDataStream &operator << ( QDataStream &os, const SdLibraryReference &ref )
  {
  os << ref.mCreationTime << ref.mInsertionTimePrivate << ref.mInsertionTimePublic << ref.mHeaderPtr << ref.mComplaintCount;
  return os;
  }


//Read function
inline QDataStream &operator >> ( QDataStream &is, SdLibraryReference &ref )
  {
  is >> ref.mCreationTime >> ref.mInsertionTimePrivate >> ref.mInsertionTimePublic >> ref.mHeaderPtr >> ref.mComplaintCount;
  return is;
  }

#endif // SDLIBRARYREFERENCE_H
