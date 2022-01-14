/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Reference to value
*/
#ifndef SDSCRIPTREFERENCE_H
#define SDSCRIPTREFERENCE_H

#include "SdScriptValue.h"

class SdScriptReference : public SdScriptValue
  {
    SdScriptValue *mReference; //!< Reference to value
  public:
    SdScriptReference( SdScriptValue *ref ) : SdScriptValue(), mReference(ref) {}



    // SdM3dValue interface
  public:
    virtual char                 type() const override      { return mReference->type(); }
    virtual float                toFloat() const override   { return mReference->toFloat(); }
    virtual QColor               toColor() const override   { return mReference->toColor(); }
    virtual QString              toString() const override  { return mReference->toString(); }
    virtual QVector3D            toVertex() const override  { return mReference->toVertex(); }
    virtual QMatrix4x4           toMatrix() const override  { return mReference->toMatrix(); }
    virtual SdScriptVal3dSegment toSegment() const override { return mReference->toSegment(); }
    virtual SdScriptVal3dPath    toPath() const override    { return mReference->toPath(); }
    virtual SdScriptVal3dRegion  toRegion() const override  { return mReference->toRegion(); }
    virtual SdScriptVal3dFace    toFace() const override    { return mReference->toFace(); }
    virtual SdScriptVal3dModel   toModel() const override   { return mReference->toModel(); }
  };

#endif // SDSCRIPTREFERENCE_H
