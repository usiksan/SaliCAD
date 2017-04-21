/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDPITEMCOMPONENT_H
#define SDPITEMCOMPONENT_H

#include "SdProjectItem.h"

#define SD_TYPE_COMPONENT "Comp"

class SdPItemPart;

class SdPItemComponent : public SdProjectItem
  {
    SdPItemPart *mPart;
  public:
    SdPItemComponent();

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;

    // SdProjectItem interface
  public:
    virtual QString getIconName() const override;
  };

#endif // SDPITEMCOMPONENT_H
