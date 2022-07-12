#ifndef IGESENTITYCOLOR_H
#define IGESENTITYCOLOR_H

#include "IgesEntity.h"

#include <QColor>

class IgesEntityColor : public IgesEntity
  {
    QColor mColor;
  public:
    IgesEntityColor();

    QColor color() const { return mColor; }

    // IgesEntity interface
  public:
    virtual bool parse(IgesReader *reader) override;
  };

#endif // IGESENTITYCOLOR_H
