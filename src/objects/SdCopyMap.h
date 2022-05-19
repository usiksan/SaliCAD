/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Object map for copy operation
*/
#ifndef SDCOPYMAP_H
#define SDCOPYMAP_H

#include "SdObject.h"

#include <QMap>


class SdCopyMap
  {
  protected:
    QMap<QString,SdObjectPtr> mObjectMap;
  public:
    SdCopyMap();

    virtual SdObjectPtr value( const QString &id, const QString &type );

    template <typename SdObjectClass>
    SdObjectClass *valueClass( const QString &id, const QString &type ) { return dynamic_cast<SdObjectClass*>( value( id, type ) ); }


    virtual SdObjectPtr copy(const SdObjectPtr src, bool next );


    template <typename SdObjectClass>
    SdObjectClass *copyClass( SdObjectClass *src, bool next ) { return dynamic_cast<SdObjectClass*>( copy( src, next ) ); }

    template <typename SdObjectClass>
    SdObjectClass *copyClass( SdObject *src, bool next ) { return dynamic_cast<SdObjectClass*>( copy( src, next ) ); }

  };

#endif // SDCOPYMAP_H
