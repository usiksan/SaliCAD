/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for ident of component moving in the plate
*/
#ifndef SDMODECPLATEIDENTMOVE_H
#define SDMODECPLATEIDENTMOVE_H

#include "SdModeCSheetIdentMove.h"



class SdModeCPlateIdentMove : public SdModeCSheetIdentMove
  {
  public:
    SdModeCPlateIdentMove(SdWEditorGraph *editor, SdProjectItem *obj);

    // SdMode interface
  public:
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getIndex() const override;

  protected:
    virtual void    setProp( const SdPropText &prp, SdPoint pos, SdUndo *undo ) override;
    virtual void    getProp() override;
    virtual quint64 getMask() const override { return dctPartImp; }
  };

#endif // SDMODECPLATEIDENTMOVE_H
