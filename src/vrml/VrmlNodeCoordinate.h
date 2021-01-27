#ifndef VRMLNODECOORDINATE_H
#define VRMLNODECOORDINATE_H

#include "VrmlNode.h"
#include "VrmlVector.h"

class VrmlNodeCoordinate : public VrmlNode
  {
    VrmlVectorList mPointList;
  public:
    VrmlNodeCoordinate();
    VrmlNodeCoordinate( const VrmlNodeCoordinate *coord ) : VrmlNode( coord ), mPointList( coord->mPointList ) {}

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeCoordinate( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODECOORDINATE_H
