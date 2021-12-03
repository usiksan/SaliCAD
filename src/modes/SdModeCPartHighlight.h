/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part hightlight mode. When pressend on component all same components are highlinghted
*/
#ifndef SDMODECPARTHIGHLIGHT_H
#define SDMODECPARTHIGHLIGHT_H

#include "SdModeCommon.h"

class SdModeCPartHighlight : public SdModeCommon
  {
    QString         mHighlightComponentBom; //!< BOM line of component being highlighted
    SdObjectPtrList mBehindCursorTable;     //!< List of components behing cursor [Список объектов под курсором]
    SdGraphPartImp* mBehindCursorPrt;       //!< Component being highlighted on left button [Компонент, подлежащий выделению при нажатии левой кнопки мыши]
    int             mBehindCursorIndex;     //!< Index of component within list of components behind cursor [Индекс объекта среди объектов под курсором]
  public:
    SdModeCPartHighlight( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual void    keyDown(int key, QChar ch) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
  };

#endif // SDMODECPARTHIGHLIGHT_H
