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

#include <QAbstractItemModel>
#include <QList>

class SdProject : public QAbstractItemModel
  {
    Q_OBJECT


  public:
    SdProject();
  };


typedef SdProject *SdProjectPtr;

typedef QList<SdProjectPtr> SdProjectPtrList;

#endif // SDPROJECT_H
