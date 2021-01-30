#ifndef VRMLNODE1MATERIALBINDING_H
#define VRMLNODE1MATERIALBINDING_H

#include "VrmlNode1.h"

#define MATERIAL_BINDING_DEFAULT            0 //Use default binding
#define MATERIAL_BINDING_OVERALL            1 //Whole object has same material
#define MATERIAL_BINDING_PER_PART           2 //One material for each part of object
#define MATERIAL_BINDING_PER_PART_INDEXED   3 //One material for each part, indexed
#define MATERIAL_BINDING_PER_FACE           4 //One material for each face of object
#define MATERIAL_BINDING_PER_FACE_INDEXED   5 //One material for each face, indexed
#define MATERIAL_BINDING_PER_VERTEX         6 //One material for each vertex of object
#define MATERIAL_BINDING_PER_VERTEX_INDEXED 7 //One material for each vertex, indexed

class VrmlNode1MaterialBinding : public VrmlNode1
  {
    int mBinding;
  public:
    VrmlNode1MaterialBinding();

    int binding() const { return mBinding; }

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1MATERIALBINDING_H
