/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for delete plate trace road

  When moving cursor we find nearest point to delete segment. This point is road-middle or road end.

  When press left button we delete segment.

*/
#ifndef SDMODECTRACEDELETE_H
#define SDMODECTRACEDELETE_H

#include "SdModeCommon.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdBarrier.h"
#include "objects/SdPropRoad.h"
#include "objects/SdRuleBlock.h"
#include "objects/SdSelector.h"
#include "objects/SdGraphTracedRoad.h"


class SdModeCTraceDelete : public SdModeCommon
  {
    //SdGraphTraced  *mToDelete; //!< Traced object appear to delete
    SdSelector      mFragment; //!< Traced objects found near current point
    SdPoint         mFirst;
    bool            mShift;    //!< With shift we delete rectangle
  public:
    SdModeCTraceDelete( SdWEditorGraph *editor, SdProjectItem *obj );
    ~SdModeCTraceDelete() override;

    // SdMode interface
  public:
//    virtual void    activate() override;
    virtual void    deactivate() override;
    virtual void    reset() override;
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    beginDrag(SdPoint p) override;
    virtual void    dragPoint(SdPoint p) override;
    virtual void    stopDrag(SdPoint p) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
    virtual void    keyDown(int key, QChar ch) override;
    virtual void    keyUp(int key, QChar ch) override;

  private:
    //Plate where road being inserted
    SdPItemPlate   *plate() { return dynamic_cast<SdPItemPlate*>(mObject); }

  };

#endif // SDMODECTRACEDELETE_H
