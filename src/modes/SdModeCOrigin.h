/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for symbol pin origin
*/
#ifndef SDMODECORIGIN_H
#define SDMODECORIGIN_H

#include "SdModeCommon.h"

class SdModeCOrigin : public SdModeCommon
  {
  protected:
    int mOriginSize; //Size for drawing origin symbol
    int mModeIndex;
  public:
    SdModeCOrigin( SdWEditorGraph *editor, SdProjectItem *obj, int osize );

    // SdMode interface
  public:
    virtual void    activate() override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint enter) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
  };

#endif // SDMODECORIGIN_H
