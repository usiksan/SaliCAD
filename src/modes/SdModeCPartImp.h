/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for part implement
*/
#ifndef SDMODECPARTIMP_H
#define SDMODECPARTIMP_H

#include "SdModeCommon.h"

class SdPItemSymbol;
class SdPItemComponent;
class SdPItemPart;

class SdModeCPartImp : public SdModeCommon
  {
    SdPoint           mOrigin;
    SdPItemPart      *mPart;
  public:
    SdModeCPartImp( SdWEditorGraph *editor, SdProjectItem *obj );
    ~SdModeCPartImp() override;

    // SdMode interface
  public:
    virtual void    activate() override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual void    keyDown(int key, QChar ch) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  private:
    //Show dialog with user part selection
    void getPart();

    //Clear previous part
    void clear();

  };

#endif // SDMODECPARTIMP_H
