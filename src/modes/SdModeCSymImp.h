/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for symbol implement
*/
#ifndef SDMODECSYMIMP_H
#define SDMODECSYMIMP_H

#include "SdModeCommon.h"

class SdPItemSymbol;
class SdPItemComponent;
class SdPItemPart;

class SdModeCSymImp : public SdModeCommon
  {
    SdPoint           mOrigin;
    SdPItemSymbol    *mSection;
    SdPItemComponent *mComponent;
    SdPItemPart      *mPart;
    SdStringMap       mParams;
  public:
    SdModeCSymImp( SdWEditorGraph *editor, SdProjectItem *obj );
    ~SdModeCSymImp() override;



    // SdMode interface
  public:
    virtual void    activate() override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual void    keyDown(int key, QChar ch) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  private:
    void getSection();
    void clear();
  };

#endif // SDMODECSYMIMP_H