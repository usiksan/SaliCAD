#ifndef VRMLNODECOORDINATE_H
#define VRMLNODECOORDINATE_H

#include "VrmlNode.h"
#include "VrmlVector.h"

class VrmlNodeCoordinate : public VrmlNode
  {
    VrmlVectorList     mPointList;
    mutable QList<int> mIndexConvertor;
  public:
    VrmlNodeCoordinate();
    VrmlNodeCoordinate( const VrmlNodeCoordinate *coord ) : VrmlNode( coord ), mPointList( coord->mPointList ) {}

    VrmlVector        at( int index ) const { return mPointList.at(index); }

    int               modelIndex( int index ) const { return mIndexConvertor.at(index); }

    virtual void      generateFaces( Sd3drModel *model, Sd3drInstance &instance, Sd3drBody &body ) const override;

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeCoordinate( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODECOORDINATE_H
