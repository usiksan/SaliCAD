/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPolyWindowList.h"
#include <QJsonArray>


QJsonArray SdPolyWindowList::write()
  {
  QJsonArray arr;
  for( int i = 0; i < count(); i++ )
    arr.append( at(i).write() );
  return arr;
  }

void SdPolyWindowList::write(const QString name, QJsonObject &obj)
  {
  obj.insert( name, write() );
  }

void SdPolyWindowList::read(const QJsonArray array)
  {
  clear();
  SdPolyWindow win;
  for( int i = 0; i < array.count(); i++ ) {
    win.read( array.at(i).toObject() );
    append( win );
    }
  }

void SdPolyWindowList::read(const QString name, const QJsonObject obj)
  {
  read( obj.value(name).toArray() );
  }

