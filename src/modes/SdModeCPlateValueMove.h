/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for value of component moving in the plate
*/
#ifndef SDMODECPLATEVALUEMOVE_H
#define SDMODECPLATEVALUEMOVE_H

#include "SdModeCPlateIdentMove.h"


class SdModeCPlateValueMove : public SdModeCPlateIdentMove
  {
  public:
    SdModeCPlateValueMove(SdWEditorGraph *editor, SdProjectItem *obj);

    // SdMode interface
  public:
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getIndex() const override;

  protected:
    virtual void    setProp( const SdPropText &prp, SdPoint pos, SdUndo *undo ) override;
    virtual void    getProp() override;
  };

#endif // SDMODECPLATEVALUEMOVE_H
