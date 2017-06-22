/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDMODECWIRENAME_H
#define SDMODECWIRENAME_H

#include "SdModeCommon.h"

class SdContainerSheetNet;
class SdPItemSheet;


class SdModeCWireName : public SdModeCommon
  {
    QString              mName;        //Wire name
    SdContainerSheetNet *mNet;         //Enter net
    SdContainerSheetNet *mShow;        //Net for show

    static SdPoint       mSmartOrigin; //Offset of net name position
    enum { sSelectNet, sPlaceName };
  public:
    SdModeCWireName( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void drawStatic(SdContext *ctx) override;
    virtual void drawDynamic(SdContext *ctx) override;
    virtual int getPropBarId() const override;
    virtual void propGetFromBar() override;
    virtual void propSetToBar() override;
    virtual void enterPoint(SdPoint) override;
    virtual void cancelPoint(SdPoint) override;
    virtual void movePoint(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeHelp() const override;
    virtual QString getStepThema() const override;
    virtual int getCursor() const override;
    virtual int getIndex() const override;

  private:
    SdPItemSheet        *getSheet();

  };

#endif // SDMODECWIRENAME_H
