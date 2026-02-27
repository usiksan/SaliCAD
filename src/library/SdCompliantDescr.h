#ifndef SDCOMPLIANTDESCR_H
#define SDCOMPLIANTDESCR_H

#include "objects/SdFileUid.h"

#include <QString>
#include <QDataStream>
#include <QList>

struct SdCompliantDescr
  {
    SdFileUid mFileUid; //!< Unical object id
    QString   mMessage; //!< Message for author
  };

using SdCompliantDescrList = QList<SdCompliantDescr>;


//Write function
inline QDataStream &operator << ( QDataStream &os, const SdCompliantDescr &ref )
  {
  os << ref.mFileUid << ref.mMessage;
  return os;
  }


//Read function
inline QDataStream &operator >> ( QDataStream &is, SdCompliantDescr &ref )
  {
  is >> ref.mFileUid >> ref.mMessage;
  return is;
  }


#endif // SDCOMPLIANTDESCR_H
