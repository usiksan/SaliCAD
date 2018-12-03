/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Shared pointer to SdProject
*/
#ifndef SDPROJECTSHAREDPTR_H
#define SDPROJECTSHAREDPTR_H

#include <QSharedPointer>

class SdProject;

typedef QSharedPointer<SdProject> SdProjectSharedPtr;

#endif // SDPROJECTSHAREDPTR_H
