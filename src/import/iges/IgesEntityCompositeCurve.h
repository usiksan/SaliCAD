#ifndef IGESENTITYCOMPOSITECURVE_H
#define IGESENTITYCOMPOSITECURVE_H

#include "IgesEntity.h"

class IgesEntityCompositeCurve : public IgesEntity
  {
    IgesEntityPtrList mList;
  public:
    IgesEntityCompositeCurve();

    // IgesEntity interface
  public:
    virtual bool parse(IgesReader *reader) override;
  };

#endif // IGESENTITYCOMPOSITECURVE_H
