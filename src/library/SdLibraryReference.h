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

#include <QDataStream>

#define SDLR_DOWNLOADED  0x1 //Object loaded from
#define SDLR_NEED_UPLOAD 0x2 //!< Object need to be upload to remote server
#define SDLR_NEED_DELETE 0x4 //!< Object need to be deleted from remote server

struct SdLibraryReference
  {
    qint32 mCreationTime;   //!< Time of object creation
    qint64 mHeaderPtr;      //!< Header reference. It contains offset from begining headers file to header of this object
    qint8  mFlags;          //!< Flags

    //Test if reference newer than time
    bool isNewer( qint32 time ) const { return mCreationTime > time; }

    //Test if reference newer or same than time
    bool isNewerOrSame( qint32 time ) const { return mCreationTime >= time; }

    //!
    //! \brief isNeedUpload Returns true if object need to be upload to remote server
    //! \return             true if object need to be upload to remote server
    //!
    bool isNeedUpload() const { return mFlags & SDLR_NEED_UPLOAD; }

    //!
    //! \brief uploadReset Reset flag upload
    //!
    void uploadReset() { mFlags &= ~SDLR_NEED_UPLOAD; }

    //!
    //! \brief isNeedDelete Return true if object need to be removed from remote server
    //! \return             true if object need to be removed from remote server
    //!
    bool isNeedDelete() const { return mFlags & SDLR_NEED_DELETE; }

    //!
    //! \brief deleteSet Set flag delete
    //!
    void deleteSet() { mFlags |= SDLR_NEED_DELETE; }
  };

//Write function
inline QDataStream &operator << ( QDataStream &os, const SdLibraryReference &ref )
  {
  os << ref.mCreationTime << ref.mHeaderPtr << ref.mFlags;
  return os;
  }


//Read function
inline QDataStream &operator >> ( QDataStream &is, SdLibraryReference &ref )
  {
  is >> ref.mCreationTime >> ref.mHeaderPtr >> ref.mFlags;
  return is;
  }

#endif // SDLIBRARYREFERENCE_H
