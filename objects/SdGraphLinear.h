/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object with linear type (line, rect, circle and so).
*/
#ifndef SDOBJECTGRAPHLINEAR_H
#define SDOBJECTGRAPHLINEAR_H

#include "SdGraph.h"
#include "SdPropLine.h"

class SdGraphLinear : public SdGraph
  {
  protected:
    SdPropLine mProp; //Linear graph object props
  public:
    SdGraphLinear();
    SdGraphLinear( const SdPropLine &propLine );

    virtual void cloneFrom(const SdObject *src) override;
    virtual void writeObject(QJsonObject &obj) const override;
    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;

    //Изменение свойствами
    virtual void setProp(SdPropSelected &prop) override;
    virtual void getProp(SdPropSelected &prop) override;

    //Проверить доступность объекта
    virtual bool isVisible() override;

    virtual void setLayerUsage() override;

    friend class SdPasCadImport;
  };

#endif // SDOBJECTGRAPHLINEAR_H
