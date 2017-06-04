/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing object - pin. It is reference on component pin
*/
#ifndef SDGRAPHROADPIN_H
#define SDGRAPHROADPIN_H

#include "SdGraphRoad.h"
#include "SdPoint.h"

#define SD_TYPE_ROAD_PIN "RoadPin"

class SdGraphArea;
class SdGraphSymPin;
class SdPItemSymbol;
class SdPItemSheet;
class SdPItemPart;
class SdGraphPartImp;
class SdPItemPlate;

class SdGraphRoadPin : public SdGraphRoad
  {
    SdGraphPartImp *mPartImp;  //Part implement containing pin
    int             mPinIndex; //Pin index int part implement
  public:
    SdGraphRoadPin();
    SdGraphRoadPin( SdGraphPartImp *part, int index );

    // SdObject interface
  public:
    virtual QString getType() const override { return SD_TYPE_ROAD_PIN; }
    virtual quint64 getClass() const override { return dctRoadPin; }
    virtual void    attach(SdUndo *undo) override;
    virtual void    detach(SdUndo *undo) override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual SdRect getOverRect() const override;

    // SdGraphRoad interface
  public:
    virtual bool isPointOnRoad(SdPoint p, SdLayer *layer) override;
    virtual bool isPinOnRoad(SdPoint p, int pinSide) override;
    virtual void accumLinked(SdPoint a, SdPoint b, SdLayer *layer, SdSelector *sel) override;
    virtual SdPoint getNetPoint() override;
  };

#endif // SDGRAPHROADPIN_H
