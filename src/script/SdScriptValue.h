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
    virtual SdScriptVal2dGraph   toGraph() const { return SdScriptVal2dGraph( QVector3D(), QVector3D() ); }

    //!
    //! \brief toIndex Convert object to index list in reference list
    //! \return        Index list in reference list
    //!
    virtual QList<int>           toIndexList() const { return QList<int>(); }

    int                          toIndex() const { return toIndexList().at(0); }
  };

using SdScriptValuePtr = SdScriptValue*;

using SdScriptValuePtrList = QList<SdScriptValuePtr>;

#endif // SDSCRIPTVALUE_H
