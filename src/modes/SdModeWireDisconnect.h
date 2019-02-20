/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode to disconnect pin from wire
*/
#ifndef SDMODEWIREDISCONNECT_H
#define SDMODEWIREDISCONNECT_H

#include "SdMode.h"

class SdPItemSheet;

class SdModeWireDisconnect : public SdMode
  {
    SdPoint         mFirst;      //First point of rectangle select
    SdPoint         mCurPoint;   //Current point of rectangle select

    //Available mode steps
    enum SdSteps : int {
      sSiglePin,                 //Disconnect single pin by single button press
      sRectangle };              //Disconnect many pins by over rectangle

    SdPItemSheet   *getSheet() const;
  public:
    SdModeWireDisconnect( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    reset() override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint enter ) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    beginDrag(SdPoint point) override;
    virtual void    dragPoint(SdPoint point) override;
    virtual void    stopDrag(SdPoint point) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  private:
    void unconnect( SdRect over );
  };

#endif // SDMODEWIREDISCONNECT_H
