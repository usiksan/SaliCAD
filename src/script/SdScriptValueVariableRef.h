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

  Referenced variable. It does not contain the value itself. Instead, it contains a reference to an
  external parameter, which contains the value. This parameter can be a line in the visual table or
  a parameter of some component, depending on the link. All links are located in the link map, the
  pointer to which contains the variable. The key in the map is the variable name.
*/
#ifndef SDSCRIPTVALUEVARIABLEREF_H
#define SDSCRIPTVALUEVARIABLEREF_H

#include "SdScriptValueVariable.h"
#include "SdScriptRefMap.h"

class SdScriptValueVariableRef : public SdScriptValueVariable
  {
    SdScriptRefMap *mRef;        //!< Reference map which contains fact reference to this variable for visual edit and view
    QString         mName;       //!< Name of variable in script. Is also used to point in reference map
    QString         mDimension;  //!< Short dimension of phisical value in this variable ("Om", "V", "Hz" and so on)
    QString         mRow;        //!< Standard row of values to which must be adjusted this variable ("E6", "E12" and so on)
                                 //!  If row is empty then it not used
  public:
    SdScriptValueVariableRef( SdScriptRefMap *ref, const QString &name, const QString &dimension, const QString &row );

    // SdScriptValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    //! For referenced variables we support only float since the use of physical quantities is assumed
    virtual char  type() const override { return SD_SCRIPT_TYPE_FLOAT; }

    //!
    //! \brief toFloat Convert object ot float value
    //! \return        float value
    //!
    //! We extract value from external param referenced by this variable
    virtual float toFloat() const override;

    //!
    //! \brief assign Assign new value to this variable. We take source value and write it to reference
    //! \param src    Source value
    //!
    virtual void  assign(SdScriptValuePtr src) override;
  };

#endif // SDSCRIPTVALUEVARIABLEREF_H
