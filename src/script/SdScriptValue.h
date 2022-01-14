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
    virtual bool                 toBool() const { return false; }

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
    virtual SdScriptVal2dGraph   toGraph() const { return SdScriptVal2dGraph( QVector3D(), QVector3D() ); }

    //!
    //! \brief toVertex Convert object to 3d vertex
    //! \return         3d vertex
    //!
    virtual QVector3D            toVertex() const { return QVector3D(); }

    virtual QMatrix4x4           toMatrix() const { return QMatrix4x4(); }

    virtual SdScriptVal3dSegment toSegment() const { return SdScriptVal3dSegment(); }

    //!
    //! \brief toPath Convert object to path of 3d segments which may be separated
    //! \return       Path of 3d segments which may be separated
    //!
    virtual SdScriptVal3dPath    toPath() const { return SdScriptVal3dPath(); }

    //!
    //! \brief toRegion Convert object to 3d region which is closed region of 3d vertex
    //! \return         3d region which is closed region of 3d vertex
    //!
    virtual SdScriptVal3dRegion  toRegion() const { return SdScriptVal3dRegion(); }

    //!
    //! \brief toFace Convert object to 3d face which is bound region and face color
    //! \return       3d face which is bound region and face color
    //!
    virtual SdScriptVal3dFace    toFace() const { return SdScriptVal3dFace(); }

    //!
    //! \brief toModel Convert object to 3d model
    //! \return        3d model
    //!
    virtual SdScriptVal3dModel   toModel() const { return SdScriptVal3dModel(); }
  };

using SdScriptValuePtr = SdScriptValue*;

using SdScriptValuePtrList = QList<SdScriptValuePtr>;

#endif // SDSCRIPTVALUE_H
