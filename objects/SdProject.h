/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPROJECT_H
#define SDPROJECT_H

#include "SdOwner.h"
#include "SdProjectItem.h"
#include <QList>

class SdProject : public SdOwner
  {
  public:
    SdProject();

    int            countOf( quintptr id );
    SdProjectItem* item( quintptr classId, int row );
  };


typedef SdProject *SdProjectPtr;

typedef QList<SdProjectPtr> SdProjectPtrList;

#endif // SDPROJECT_H
