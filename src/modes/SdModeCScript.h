#ifndef SDMODECSCRIPT_H
#define SDMODECSCRIPT_H

#include "SdMode.h"

class SdModeCScript : public SdMode
  {
    SdPropText *mPropText;     //!< Text properties
    const int sPlace = 0, sLink = 1, sOver = 2;
  public:
    SdModeCScript( SdWEditorGraph *editor, SdProjectItem *obj );


    // SdMode interface
  public:
    virtual void activate() override;
    virtual void deactivate() override;
    virtual void reset() override;
    virtual void drawStatic(SdContext *ctx) override;
    virtual void drawDynamic(SdContext *ctx) override;
    virtual int getPropBarId() const override;
    virtual void propGetFromBar() override;
    virtual void propSetToBar() override;
    virtual void enterPoint(SdPoint) override;
    virtual void clickPoint(SdPoint) override;
    virtual void cancelPoint(SdPoint) override;
    virtual void movePoint(SdPoint) override;
    virtual bool wheel(SdPoint) override;
    virtual void keyDown(int key, QChar ch) override;
    virtual void keyUp(int key, QChar ch) override;
    virtual SdPoint enterPrev() override;
    virtual void beginDrag(SdPoint) override;
    virtual void dragPoint(SdPoint) override;
    virtual void stopDrag(SdPoint) override;
    virtual bool getInfo(SdPoint p, QString &info) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int getCursor() const override;
    virtual int getIndex() const override;

  private:
  };

#endif // SDMODECSCRIPT_H
