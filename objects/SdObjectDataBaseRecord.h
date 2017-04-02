/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDOBJECTDATABASERECORD_H
#define SDOBJECTDATABASERECORD_H

#include "SdObjectInfo.h"

struct SdObjectDataBaseRecord : public SdObjectInfo
  {
    int mDirectory; //Dir where object file is

    SdObjectDataBaseRecord();
  };

QDataStream& operator << ( QDataStream &os, const SdObjectDataBaseRecord &info );
QDataStream& operator >> ( QDataStream &is, SdObjectDataBaseRecord &info );

#endif // SDOBJECTDATABASERECORD_H
