/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphics representation part of component
*/
#ifndef SDPITEMPART_H
#define SDPITEMPART_H

#include "SdProjectItem.h"

#define SD_TYPE_PART "Part"

class SdPItemPart : public SdProjectItem
  {
  public:
    SdPItemPart();

    // SdObject interface
  public:
    virtual QString        getType() const override;
    virtual quint64        getClass() const override;

    // SdProjectItem interface
  public:
    virtual QString        getIconName() const override;
    virtual bool           is3dAllowed() const override { return true; }
    virtual quint64        getAcceptedObjectsMask() const override;
  };

#endif // SDPITEMPART_H
