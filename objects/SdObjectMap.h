/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDOBJECTMAP_H
#define SDOBJECTMAP_H

#include <QMap>

class SdObject;

typedef SdObject *SdObjectPtr;

typedef QMap<int,SdObjectPtr> SdObjectMap;

#endif // SDOBJECTMAP_H
