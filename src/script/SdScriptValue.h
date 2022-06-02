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

  Value of some type. It is base class for each types of values
*/
#ifndef SDSCRIPTVALUE_H
#define SDSCRIPTVALUE_H

#include "SdScriptTypes.h"

class SdScriptValue
  {
  public:
    SdScriptValue() {}
    virtual ~SdScriptValue() {}

    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char                 type() const { return SD_SCRIPT_TYPE_NONE; }

    //!
    //! \brief toBool Convert object to bool value
    //! \return       bool value
    //!
    virtual bool                 toBool() const { return toFloat() > 0.0; }

    //!
    //! \brief toFloat Convert object ot float value
    //! \return        float value
    //!
    virtual float                toFloat() const { return 0.0; }

    //!
    //! \brief toColor Convert object to color
    //! \return        Color
    //!
    virtual QColor               toColor() const { return QColor(); }

    //!
    //! \brief toString Convert object to string
    //! \return         String
    //!
    virtual QString              toString() const { return QString(); }

    //!
    //! \brief toGraph Convert object to 2d graph
    //! \return        2d graph
    //!
    virtual SdScriptVal2dGraph   toGraph() const { return SdScriptVal2dGraph( toFloatList() ); }

    //!
    //! \brief toMatrix Convert object to 4x4 matrix
    //! \return         4x4 matrix
    //!
    virtual QMatrix4x4           toMatrix() const { return QMatrix4x4{}; }

    //!
    //! \brief toIndex Convert object to float list
    //! \return        Float list
    //!
    virtual QList<float>         toFloatList() const { return QList<float>(); }

    //!
    //! \brief toVertexIndex Convert object to vertex index in Sd3dModel point list
    //! \return              Vertex index in Sd3dModel point list
    //!
    virtual int                  toVertexIndex() const { return 0; }

    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace            toFace() const { return Sd3drFace(); }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList        toFaceList() const { return Sd3drFaceList(); }

    //!
    //! \brief toModel Convert object to model which is compound of some bodies
    //! \return        Model which is compound of some bodies
    //!
    virtual Sd3drInstance        toModel() const { return Sd3drInstance(); }

  };

using SdScriptValuePtr = SdScriptValue*;

using SdScriptValuePtrList = QList<SdScriptValuePtr>;

#endif // SDSCRIPTVALUE_H
