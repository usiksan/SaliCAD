/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDLIBRARYSECTION_H
#define SDLIBRARYSECTION_H

#include <QDataStream>

struct SdLibraryCategory {
    QString mAssociation;
    qint32  mCreationIndex;
  };

inline QDataStream &operator << ( QDataStream &os, const SdLibraryCategory &section ) {
  os << section.mAssociation << section.mCreationIndex;
  return os;
  }

inline QDataStream &operator >> ( QDataStream &is, SdLibraryCategory &section ) {
  is >> section.mAssociation >> section.mCreationIndex;
  return is;
  }

#endif // SDLIBRARYSECTION_H
