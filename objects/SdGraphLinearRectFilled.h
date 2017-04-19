/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object filled rectangle.
*/
#ifndef SDGRAPHLINEARRECTFILLED_H
#define SDGRAPHLINEARRECTFILLED_H

#include "SdGraphLinearRect.h"

#define SD_TYPE_RECT_FILLED "RectFilled"

class SdGraphLinearRectFilled : public SdGraphLinearRect
  {
  public:
    SdGraphLinearRectFilled();
    SdGraphLinearRectFilled( SdPoint p1, SdPoint p2, const SdPropLine &propLine );

    // SdObject interface
    virtual QString getType() const override { return QString(SD_TYPE_RECT_FILLED); }

    // SdGraph interface
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
  };

#endif // SDGRAPHLINEARRECTFILLED_H
