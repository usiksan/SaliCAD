#ifndef IGESENTITYFACEGENERATOR_H
#define IGESENTITYFACEGENERATOR_H

#include "IgesEntity.h"

class IgesEntityFaceGenerator : public IgesEntity
  {
  public:
    IgesEntityFaceGenerator();

    virtual void generateFaces( Sd3drModel *model ) = 0;
  };

#endif // IGESENTITYFACEGENERATOR_H
