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
#ifndef SDGRAPHROAD_H
#define SDGRAPHROAD_H

#include "SdGraph.h"

class SdContainerPlateNet;
class SdPItemPlate;

class SdGraphRoad : public SdGraph
  {
  public:

    //Subnet iteract
    int mSubNetIndex; //In tracing process some segments may construct  separate subnets
                      //This index is for distancing one subnet from another

    SdGraphRoad();

    //Определены здесь
    SdContainerPlateNet *getNet() const;
    QString              getNetName() const;
    SdPItemPlate        *getPlate();
    virtual bool         isPointOnRoad( SdPoint p, SdLayer *layer ) = 0;
    virtual bool         isPinOnRoad( SdPoint p, int pinSide ) = 0;
    virtual void         accumLinked( SdPoint a, SdPoint b, SdLayer *layer, SdSelector *sel ) = 0;
    virtual SdPoint      getNetPoint() = 0;

//    virtual bool         GetInfo( DPoint p, DString &info, bool extInfo );
//    virtual void  CalcSubNetIndex( DVector<DRoadPic> *segments, int from, int &count ) = 0;

    // SdObject interface
  public:
    virtual void cloneFrom(const SdObject *src) override;
    virtual void writeObject(QJsonObject &obj) const override;
    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;
  };

#endif // SDGRAPHROAD_H
