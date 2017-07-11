/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
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
    virtual QString getType() const override;
    virtual quint64 getClass() const override;

    // SdProjectItem interface
  public:
    virtual QString       getIconName() const override;
    virtual quint64       getAcceptedObjectsMask() const override;
    virtual SdGraphIdent *createIdent() override;
    virtual void          insertObjects( SdSelector *sel, SdUndo *undo, SdWEditorGraph *editor, SdSelector *dest ) override;
  };

#endif // SDPITEMPART_H
