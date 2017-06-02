/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base for all tracing object: trace, polygon, via, wire
*/
#include "SdGraphRoad.h"
#include "SdContainerPlateNet.h"

SdGraphRoad::SdGraphRoad() :
  SdGraph(),
  mSubNetIndex(-1)
  {

  }

SdContainerPlateNet *SdGraphRoad::getNet() const
  {
  return dynamic_cast<SdContainerPlateNet*>(getParent());
  }

QString SdGraphRoad::getNetName() const
  {
  return getNet()->getNetName();
  }

SdPItemPlate *SdGraphRoad::getPlate()
  {
  return getNet()->getPlate();
  }


void SdGraphRoad::cloneFrom(const SdObject *src)
  {
  SdGraph::cloneFrom(src);
  const SdGraphRoad *road = dynamic_cast<const SdGraphRoad*>(src);
  Q_ASSERT( road != nullptr );
  mSubNetIndex = road->mSubNetIndex;
  }



void SdGraphRoad::writeObject(QJsonObject &obj) const
  {
  SdGraph::writeObject(obj);
  obj.insert( QStringLiteral("SubNet"), mSubNetIndex );
  }




void SdGraphRoad::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mSubNetIndex = obj.value( QStringLiteral("SubNet") ).toInt();
  }
