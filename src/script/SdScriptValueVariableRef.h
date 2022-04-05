#ifndef SDSCRIPTVALUEVARIABLEREF_H
#define SDSCRIPTVALUEVARIABLEREF_H

#include "SdScriptValueVariable.h"

class SdScriptValueFunRef;

class SdScriptValueVariableRef : public SdScriptValueVariable
  {
    SdScriptValueFunRef *mFunRef;  //!< Reference function to access to param
  public:
    SdScriptValueVariableRef();

    // SdScriptValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char  type() const override { return SD_SCRIPT_TYPE_FLOAT; }

    //!
    //! \brief toFloat Convert object ot float value
    //! \return        float value
    //!
    virtual float toFloat() const override;

    virtual void  assign(SdScriptValuePtr src) override;
  };

#endif // SDSCRIPTVALUEVARIABLEREF_H
