/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for text edit
*/
#ifndef SDMODECTEXT_H
#define SDMODECTEXT_H

#include "SdModeCTextual.h"
#include "objects/SdGraph.h"

class SdModeCText : public SdModeCTextual
  {
    SdGraph   *mPicAffected; //Элемент, содержащий редактируемый текст
    int        mIdAffected;  //Номер редактируемого текста в элементе
    SdPropText mEditText;    //Properties for edited text
    const int sPlace = 0, sEdit = 1, sEnter = 2;
  public:
    SdModeCText( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawStatic( SdContext *ctx ) override;  //Ресует постоянную часть картинки
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeHelp() const override;
    virtual QString getStepThema() const override;
    virtual int     getIndex() const override;

    // SdModeCTextual interface
  protected:
    virtual void    cancelEdit() override;
    virtual void    applyEdit() override;
  };

#endif // SDMODECTEXT_H
