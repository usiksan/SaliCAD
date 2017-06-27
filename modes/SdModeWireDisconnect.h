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
    SdPItemSheet *getSheet() const;
  public:
    SdModeWireDisconnect( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void enterPoint(SdPoint enter ) override;
    virtual void cancelPoint(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int getCursor() const override;
    virtual int getIndex() const override;
  };

#endif // SDMODEWIREDISCONNECT_H
