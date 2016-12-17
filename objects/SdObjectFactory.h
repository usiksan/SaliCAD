/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDOBJECTFACTORY_H
#define SDOBJECTFACTORY_H

#include "SdObject.h"
#include <QString>
#include <QList>

struct SdObjectDataBaseRecord {
    QString mName;   //Наименование объекта
    QString mId;     //Идентификатор объекта
    bool    mLocal;  //Когда истина, то имеется локальная копия
  };


typedef SdObjectDataBaseRecord *SdObjectDataBaseRecordPtr;

typedef QList<SdObjectDataBaseRecordPtr> SdObjectDataBaseRecordPtrList;

class SdObjectFactory
  {
    //static QMap
  public:
    SdObjectFactory();

    static void addToFactory( const SdObject *obj );

    static void fillObjectList( const QString filtr, SdObjectDataBaseRecordPtrList *list );
  };

#endif // SDOBJECTFACTORY_H
