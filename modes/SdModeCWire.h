/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for enter wire in sheet
*/
#ifndef SDMODECWIRE_H
#define SDMODECWIRE_H

#include "SdModeCommon.h"
#include "objects/SdPropLine.h"

class SdContainerSheetNet;
class SdPItemSheet;

class SdModeCWire : public SdModeCommon
  {
    SdPoint              mFirst;     //First wire point
    SdPoint              mPrevMove;  //Previous move point
    SdPoint              mMiddle;    //Vertex point
    //int           enterType; //Режим ввода линий
    QString              mName;      //Wire name
    SdContainerSheetNet *mNet;       //Enter net
    SdContainerSheetNet *mShow;      //Net for show

    SdPoint              mStrEnd,    //Stringet of last point of autonet
                         mSmA,       //Intermediate point of autonet
                         mSmEnd;     //Last point of autonet

    const int sFirstPoint = 0, sNextPoint = 1;

  public:
    enum RenumResult { renCancel, renFirst, renSecond };

    SdModeCWire( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    activate() override;
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual void    enterPrev() override;
    //virtual bool getInfo(SdPoint p, QString &info) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  protected:
    RenumResult          getUnionResult( const QString firstWireName, const QString secondWireName );
    bool                 okUnion( const QString newName );
    void                 renameOtherSheets(const QString wireName );

    bool                 testFirstPoint( SdPoint p );
    bool                 testNextPoint( SdPoint p );
    void                 renameNet( const QString sour, const QString dest );
//    void        SetSourNet( PDBasePic pic );
    void                 calcSecondSmart();
    void                 calcSmartPoint();
    void                 nextNet();
    SdContainerSheetNet *needCurNet();
    SdPItemSheet        *getSheet();

  };

#endif // SDMODECWIRE_H
