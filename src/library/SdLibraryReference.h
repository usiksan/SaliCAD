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

#define SDLR_LOCAL_CHANGED  0x1 //!< Object changed locally
#define SDLR_PUBLIC         0x2 //!< Object is public and must be uploaded to remote global server
#define SDLR_REMOVE_GLOBAL  0x4 //!< Object need to be removed from global server
#define SDLR_REMOVE_PRIVATE 0x8 //!< Object need to be removed from private sync cloud

struct SdLibraryFileUid : public SdFileUid
  {
    qint32 mInsertionTime;  //!< Time of object insertion into library
    qint8  mFlags;          //!< Flags

  };


struct SdLibraryReference
  {
    qint32 mCreationTime;   //!< Time of object creation
    qint32 mInsertionTime;  //!< Time of object insertion into library
    qint64 mHeaderPtr;      //!< Header reference. It contains offset from begining headers file to header of this object
    qint8  mFlags;          //!< Flags

    SdLibraryReference() : mCreationTime(0) {}

    bool isValid() const { return mCreationTime != 0; }

    bool isRemoved() const { return mHeaderPtr == 0; }

    //!
    //! \brief isCanBeRemove Return true when reference may be removed from library reference storage
    //! \return              true when reference may be removed from library reference storage
    //!
    bool isCanBeRemove() const { return mHeaderPtr == 0 && (mFlags & (SDLR_REMOVE_GLOBAL|SDLR_REMOVE_PRIVATE)) == 0; }

    bool isInsertAfter( qint32 time ) const { return mInsertionTime > time; }

    //Test if reference newer than time
    bool isNewer( qint32 time ) const { return mCreationTime > time; }

    //Test if reference newer or same than time
    bool isNewerOrSame( qint32 time ) const { return mCreationTime >= time; }

    bool isPublic() const { return mFlags & SDLR_PUBLIC; }

    bool isPrivate() const { return (mFlags & SDLR_PUBLIC) == 0; }

    bool isLocalChanged() const { return mFlags & SDLR_LOCAL_CHANGED; }

    bool isDownloaded() const { return (mFlags & SDLR_LOCAL_CHANGED) == 0; }

    bool isRemoveGlobal() const { return mFlags & SDLR_REMOVE_GLOBAL; }

    bool isRemovePrivate() const { return mFlags & SDLR_REMOVE_PRIVATE; }

    bool isRemoveAny() const { return mFlags & (SDLR_REMOVE_GLOBAL | SDLR_REMOVE_PRIVATE); }

    QString fileName( const QString &hashUidName ) const { return SdFileUid::fileUid( hashUidName, mCreationTime ); }

    void    markAsRemoved()
      {
      mFlags |= isPublic() ? SDLR_REMOVE_GLOBAL : SDLR_REMOVE_PRIVATE;
      mHeaderPtr = 0;
      }
  };

//Write function
inline QDataStream &operator << ( QDataStream &os, const SdLibraryReference &ref )
  {
  os << ref.mCreationTime << ref.mInsertionTime << ref.mHeaderPtr << ref.mFlags;
  return os;
  }


//Read function
inline QDataStream &operator >> ( QDataStream &is, SdLibraryReference &ref )
  {
  is >> ref.mCreationTime >> ref.mInsertionTime >> ref.mHeaderPtr >> ref.mFlags;
  return is;
  }

#endif // SDLIBRARYREFERENCE_H
