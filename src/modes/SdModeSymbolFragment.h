/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Fragment insertion in symbol mode. With this we can select and insert symbol
  from library when editing current symbol. So our symbol can be as complex
  mix of other symbols and our graphics
*/
#ifndef SDMODESYMBOLFRAGMENT_H
#define SDMODESYMBOLFRAGMENT_H

#include "SdModeSelect.h"

class SdModeSymbolFragment : public SdModeSelect
  {
  public:
    SdModeSymbolFragment( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    activate() override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual int     getIndex() const override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;

  private:
    void getSymbolFragment();

  };

#endif // SDMODESYMBOLFRAGMENT_H
