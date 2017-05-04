/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Filled Circle.
*/
#ifndef SDGRAPHLINEARCIRCLEFILLED_H
#define SDGRAPHLINEARCIRCLEFILLED_H

#include "SdGraphLinearCircle.h"

#define SD_TYPE_CIRCLE_FILLED "CircleFilled"


class SdGraphLinearCircleFilled : public SdGraphLinearCircle
  {
  public:
    SdGraphLinearCircleFilled();
    SdGraphLinearCircleFilled( SdPoint center, int radius, const SdPropLine &propLine );


    // SdObject interface
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_CIRCLE_FILLED); }

    // SdGraph interface
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
  };

#endif // SDGRAPHLINEARCIRCLEFILLED_H
