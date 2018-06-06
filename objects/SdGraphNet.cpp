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

SdGraphNet::SdGraphNet() :
  SdGraph(),
  mNetName()
  {

  }

SdGraphNet::SdGraphNet(const QString netName) :
  SdGraph(),
  mNetName(netName)
  {

  }




void SdGraphNet::setNetName(const QString netName, SdUndo *undo)
  {
  //Store previous net name
  undo->string2( &mNetName, nullptr );
  mNetName = netName;
  }



void SdGraphNet::cloneFrom(const SdObject *src)
  {
  SdGraph::cloneFrom(src);
  const SdGraphNet *wire = dynamic_cast<const SdGraphNet*>(src);
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



//Set new properties to net
void SdGraphNet::setProp(SdPropSelected &prop)
  {
  prop.mWireName.assignTo( mNetName );
  }



//Get properties from net
void SdGraphNet::getProp(SdPropSelected &prop)
  {
  prop.mWireName.append( mNetName );
  }



void SdGraphNet::saveState(SdUndo *undo)
  {
  undo->string2( &mNetName, nullptr );
  }
