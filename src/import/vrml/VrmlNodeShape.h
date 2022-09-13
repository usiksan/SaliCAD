#ifndef VRMLNODESHAPE_H
#define VRMLNODESHAPE_H

#include "VrmlNode.h"

class VrmlNodeShape : public VrmlNode
  {
    VrmlNodePtr mApperance;
    VrmlNodePtr mGeometry;
  public:
    VrmlNodeShape();
    VrmlNodeShape( const VrmlNodeShape *shape );
    virtual ~VrmlNodeShape();

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeShape( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
    virtual void      generateFaces( Sd3drModel *model, Sd3drInstance &instance, Sd3drBody &body ) const override;
  };

#endif // VRMLNODESHAPE_H
