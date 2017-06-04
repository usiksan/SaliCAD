/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdContainerPlateNet.h"

SdContainerPlateNet::SdContainerPlateNet()
  {

  }



SdContainerPlateNet::SdContainerPlateNet(const QString netName) :
  SdContainer(),
  mNetName(netName)
  {

  }



SdPItemPlate *SdContainerPlateNet::getPlate() const
  {
  return dynamic_cast<SdPItemPlate*>(getParent());
  }




void SdContainerPlateNet::cloneFrom(const SdObject *src)
  {
  SdContainer::cloneFrom( src );
  const SdContainerPlateNet *net = dynamic_cast<const SdContainerPlateNet*>(src);
  Q_ASSERT( net != nullptr );
  mNetName = net->mNetName;
  }




void SdContainerPlateNet::writeObject(QJsonObject &obj) const
  {
  SdContainer::writeObject( obj );
  obj.insert( QStringLiteral("NetName"), mNetName );
  }



void SdContainerPlateNet::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdContainer::readObject( map, obj );
  mNetName = obj.value( QStringLiteral("NetName") ).toString();
  }

