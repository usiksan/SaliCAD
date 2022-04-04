#ifndef SDMODECPARAM_H
#define SDMODECPARAM_H

#include "SdModeCText.h"

class SdModeCParam : public SdModeCText
  {
  public:
    SdModeCParam( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int getIndex() const override;

    // SdModeCTextual interface
  protected:
    virtual void applyEdit() override;
  };

#endif // SDMODECPARAM_H
