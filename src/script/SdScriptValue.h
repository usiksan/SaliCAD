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

    virtual float                toFloat() const { return 0.0; }

    virtual QColor               toColor() const { return QColor(); }

    virtual QString              toString() const { return QString(); }


    virtual SdScriptVal2dGraph   toGraph() const { return SdScriptVal2dGraph( QVector3D(), QVector3D() ); }


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
