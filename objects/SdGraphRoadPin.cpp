#include "SdGraphRoadPin.h"
#include "SdContext.h"
#include "SdGraphPartImp.h"

SdGraphRoadPin::SdGraphRoadPin()
  {

  }

SdGraphRoadPin::SdGraphRoadPin(SdGraphPartImp *part, int index) :
  SdGraphRoad(),
  mPartImp(part),
  mPinIndex(index)
  {

  }




void SdGraphRoadPin::attach(SdUndo *undo)
  {
  //TODO attach road pin
  }




void SdGraphRoadPin::detach(SdUndo *undo)
  {
  //TODO detach road pin
  }




void SdGraphRoadPin::cloneFrom(const SdObject *src)
  {
  SdGraphRoad::cloneFrom( src );
  const SdGraphRoadPin *pin = dynamic_cast<const SdGraphRoadPin*>( src );
  mPartImp  = pin->mPartImp;
  mPinIndex = pin->mPinIndex;
  }



void SdGraphRoadPin::writeObject(QJsonObject &obj) const
  {
  SdGraphRoad::writeObject( obj );
  SdObject::writePtr( mPartImp, QStringLiteral("PartImp"), obj );
  obj.insert( QStringLiteral("Index"), mPinIndex );
  }




void SdGraphRoadPin::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraphRoad::readObject( map, obj );
  mPartImp = dynamic_cast<SdGraphPartImp*>( SdObject::readPtr( QStringLiteral("PartImp"), map, obj ) );
  mPinIndex = obj.value( QStringLiteral("Index") ).toInt();
  }




SdRect SdGraphRoadPin::getOverRect() const
  {
  return SdRect( mPartImp->pinPosition(mPinIndex), mPartImp->pinPosition(mPinIndex) );
  }




bool SdGraphRoadPin::isPointOnRoad(SdPoint p, SdLayer *layer)
  {
  Q_UNUSED(p)
  Q_UNUSED(layer)
  //TODO point on road for pin
  return false;
  }




bool SdGraphRoadPin::isPinOnRoad(SdPoint p, int pinSide)
  {
  Q_UNUSED(p)
  Q_UNUSED(layer)
  //TODO pin on road for pin
  return false;
  }




void SdGraphRoadPin::accumLinked(SdPoint a, SdPoint b, SdLayer *layer, SdSelector *sel)
  {
  //TODO accumLinked for pin
  }



SdPoint SdGraphRoadPin::getNetPoint()
  {
  //TODO getNetPoint
  return SdPoint();
  }
