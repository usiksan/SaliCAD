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
#include "SdObjectDataBaseRecord.h"
#include <QString>
#include <QList>

typedef SdObjectDataBaseRecord *SdObjectDataBaseRecordPtr;

typedef QList<SdObjectDataBaseRecordPtr> SdObjectDataBaseRecordPtrList;

class SdObjectFactory
  {
    static QString mLibraryPath; //Путь к библиотеке
    //static QMap
  public:
    //SdObjectFactory();

    static void setLibraryPath( const QString path );

    static void addToFactory( const SdObject *obj );

    static void fillObjectList( const QString filtr, SdObjectDataBaseRecordPtrList *list );
  };

#endif // SDOBJECTFACTORY_H
