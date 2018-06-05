/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdGraphNet.h"

SdGraphNet::SdGraphNet()
  {

  }




void SdGraphNet::setNetName(const QString netName, SdUndo *undo)
  {
  //Store previous net name
  }



void SdGraphNet::cloneFrom(const SdObject *src)
  {
  SdGraph::cloneFrom(src);
  SdGraphNet *wire = dynamic_cast<SdGraphNet*>(src);
  if( wire != nullptr )
    mNetName = wire->mNetName;
  }



void SdGraphNet::writeObject(QJsonObject &obj) const
  {
  SdGraph::writeObject(obj);
  obj.insert( QStringLiteral("netName"), mNetName );
  }




void SdGraphNet::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mNetName = obj.value( QStringLiteral("netName") ).toString();
  }
