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

    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;
    virtual void json( SdJsonWriter &js) const override;
    virtual void json(const SdJsonReader &js) override;

    //Properties service [Изменение свойствами]
    virtual void setProp(SdPropSelected &prop) override;
    virtual void getProp(SdPropSelected &prop) override;

    //Проверить доступность объекта
    virtual bool isVisible() override;

    virtual void setLayerUsage() override;

    //Compare if is layer match to object layer
            bool isMatchLayer( SdLayer *layer ) const { return mProp.mLayer.layer() == layer; }

    friend class SdPasCadImport;
  };

#endif // SDOBJECTGRAPHLINEAR_H
