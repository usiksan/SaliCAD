/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdProject.h"
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QByteArray>

SdProject::SdProject()
  {

  }

SdProject::SdProject(SdProject *src)
  {
  for( SdProjectItemPtr ptr : src->mItems )
    mItems.append( dynamic_cast<SdProjectItemPtr>(ptr->copy()) );
  }

SdProject::~SdProject()
  {
  qDeleteAll(mItems);
  }



QString SdProject::getType() const
  {
  return QString(SD_TYPE_PROJECT);
  }

int SdProject::getClass() const
  {
  return dctProject;
  }

SdObject *SdProject::copy()
  {
  return new SdProject( this );
  }

void SdProject::writeObject(QJsonObject &obj) const
  {
  SdOwner::writeObject( obj );
  QJsonArray array;
  for( SdProjectItemPtr ptr : mItems )
    array.append( ptr->write() );
  obj.insert( QStringLiteral("Items"), array );
  }

void SdProject::forEach(SdIterator &i)
  {
  for( SdProjectItemPtr ptr : mItems ) {
    if( ptr && i.isAccept(ptr->getClass()) ) {
      if( !i.operation( ptr ) )
        return;
      }
    }
  }

SdProject *SdProject::load(const QString fname)
  {
  QFile file(fname);
  if( file.open(QIODevice::ReadOnly) ) {
    QJsonDocument doc = QJsonDocument::fromJson( file.readAll(), 0 );
    if( !doc.isEmpty() ) {
      SdObjectMap map;
      return only<SdProject>( SdObject::read( &map, doc.object() ) );
      }
    }
  return 0;
  }
