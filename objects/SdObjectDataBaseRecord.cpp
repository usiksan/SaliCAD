/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdObjectDataBaseRecord.h"

SdObjectDataBaseRecord::SdObjectDataBaseRecord() :
  SdObjectInfo(),
  mDirectory(0)
  {

  }


QDataStream& operator << ( QDataStream &os, const SdObjectDataBaseRecord &info )
  {
  os << info.mAuthor
     << info.mDirectory
     << info.mCreateTime
     << info.mStatus
     << info.mTitle;
  return os;
  }

QDataStream& operator >> ( QDataStream &is, SdObjectDataBaseRecord &info )
  {
  is >> info.mAuthor
     >> info.mDirectory
     >> info.mCreateTime
     >> info.mStatus
     >> info.mTitle;
  return is;
  }
