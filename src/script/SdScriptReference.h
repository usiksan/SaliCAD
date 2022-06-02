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
    virtual char                 type() const override          { return mReference->type(); }
    virtual float                toFloat() const override       { return mReference->toFloat(); }
    virtual QColor               toColor() const override       { return mReference->toColor(); }
    virtual QString              toString() const override      { return mReference->toString(); }
    virtual int                  toVertexIndex() const override { return mReference->toVertexIndex(); }
    virtual QMatrix4x4           toMatrix() const override      { return mReference->toMatrix(); }
    virtual Sd3drFaceList        toFaceList() const override    { return mReference->toFaceList(); }
    virtual QList<float>         toFloatList() const override   { return mReference->toFloatList(); }
    virtual Sd3drFace            toFace() const override        { return mReference->toFace(); }
    virtual Sd3drInstance        toModel() const override       { return mReference->toModel(); }
  };

#endif // SDSCRIPTREFERENCE_H
