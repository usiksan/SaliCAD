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

struct SdLibraryReference
  {
    qint32 mCreationIndex;  //Index with which object is registered.
    qint32 mCreationTime;   //Time of object creation
    qint64 mHeaderPtr;      //Header reference. It contains offset from begining headers file to header of this object
    qint64 mObjectPtr;      //Object reference. It contains offset from begining objects file to object.
                            //Object reference can be null if no object loaded. Header can not be null.

    //Test if reference newer than time
    bool isNewer( qint32 time ) const { return mCreationTime > time; }

    //Test if reference newer or same than time
    bool isNewerOrSame( qint32 time ) const { return mCreationTime >= time; }

    //Test if object newer or same
    bool isObjectNewerOrSame( qint32 time ) const { return (mObjectPtr != 0 && mCreationTime >= time) || (mCreationTime > time); }
  };

//Write function
inline QDataStream &operator << ( QDataStream &os, const SdLibraryReference &ref )
  {
  os << ref.mCreationIndex << ref.mCreationTime << ref.mHeaderPtr << ref.mObjectPtr;
  return os;
  }


//Read function
inline QDataStream &operator >> ( QDataStream &is, SdLibraryReference &ref )
  {
  is >> ref.mCreationIndex >> ref.mCreationTime >> ref.mHeaderPtr >> ref.mObjectPtr;
  return is;
  }

#endif // SDLIBRARYREFERENCE_H
