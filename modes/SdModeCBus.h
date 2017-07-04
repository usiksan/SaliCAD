/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for enter bus wires
*/
#ifndef SDMODECBUS_H
#define SDMODECBUS_H

#include "SdModeCommon.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdPointList.h"

class SdModeCBus : public SdModeCommon
  {
    static SdPointList mPattern; //Bus wire pattern
    static SdPoint     mNamePos;
    //static DTextProp namProp;   //Вид имени цепи

    //DLineProp     prop;      //Свойства цепей
    //DName         name;      //Очередное имя цепи
    //int           enterType; //Тип ввода цепей
    SdPoint       mPrevMove;     //Previous cursor position point
    SdPoint       mSmartOrigin;  //Smart point when pattern enter
    int           mPrevDirection;

    QStringList   mNetList;      //Net names list
    int           mIndex;        //Current name index

    SdPointList   mSmartTable;   //Supposed bus wires placement

    SdPItemSheet* getSheet() const { return dynamic_cast<SdPItemSheet*>(mObject); }

    enum { sFirstPoint, sSecondPoint, sNextPoint, sNamePlace, sNextNet, sLast };
  public:
    SdModeCBus( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual void    enterPrev() override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
    virtual void    activate() override;

  private:
    bool    getNextNet();
    void    smartDraw( SdContext *ctx );
    bool    testNextPoint( SdPoint p );
    void    enterNet();
    void    moveAll( SdPoint p );
    SdPoint drawPattern( SdContext *ctx );
    bool    getNetList();
  };

#endif // SDMODECBUS_H
