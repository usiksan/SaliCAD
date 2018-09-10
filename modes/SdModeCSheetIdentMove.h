/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Move for ident of component moving in the schematic sheet
*/
#ifndef SDMODECSHEETIDENTMOVE_H
#define SDMODECSHEETIDENTMOVE_H

#include "SdModeCommon.h"

class SdGraphSymImp;

class SdModeCSheetIdentMove : public SdModeCommon
  {
    SdObjectPtrList mBehindCursorTable; //Список объектов под курсором
    int             mBehindCursorIndex; //Индекс объекта среди объектов под курсором
    SdGraphSymImp  *mImp;
    QTransform      mImpMatrix;
    SdPropText      mPropSaved;
    SdPoint         mPositionSaved;
    SdPropText      mPropText;
    SdPoint         mPosition;
    SdPropText      mPropSmart;

    const int sSelectComp = 0, sPlaceIdent = 1;
  public:
    SdModeCSheetIdentMove(SdWEditorGraph *editor, SdProjectItem *obj);

    // SdMode interface
  public:
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint p) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual SdPoint enterPrev() override;
    virtual bool    getInfo(SdPoint p, QString &info) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
    virtual void    keyDown(int key, QChar ch) override;
  };

#endif // SDMODECSHEETIDENTMOVE_H

