/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDMODECTEXTUAL_H
#define SDMODECTEXTUAL_H

#include "SdModeCommon.h"

class SdModeCTextual : public SdModeCommon
  {
  protected:
    QString mString;
    int     mPos;
    int     mStartSel;
    int     mStopSel;
  public:
    SdModeCTextual( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void keyDown(int key, QChar ch) override;
    virtual int getCursor() const override;
  };

#endif // SDMODECTEXTUAL_H
