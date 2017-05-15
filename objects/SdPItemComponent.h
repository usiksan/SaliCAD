/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Component is symbol without graph bun contains symbol sections with pin (name-number) assotiation and component part
*/
#ifndef SDPITEMCOMPONENT_H
#define SDPITEMCOMPONENT_H

#include "SdPItemSymbol.h"

#define SD_TYPE_COMPONENT "Comp"


class SdPItemComponent : public SdPItemSymbol
  {
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
