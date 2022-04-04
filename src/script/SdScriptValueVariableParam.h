#ifndef SDSCRIPTVALUEVARIABLEPARAM_H
#define SDSCRIPTVALUEVARIABLEPARAM_H

#include "SdScriptValueVariable.h"

class SdPItemSheet;
class SdObject;
class SdScriptValueFunParam;

class SdScriptValueVariableParam : public SdScriptValueVariable
  {
    SdPItemSheet          *mSheet;     //!< Sheet which on parameter works
    SdObject              *mComp;      //!< Component
    SdScriptValueFunParam *mFunParam;  //!< Function which declares param
    //float                  mValue;     //!< Value of parameter
  public:
    SdScriptValueVariableParam( SdPItemSheet *sheet );

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

    // SdScriptValueVariable interface
  public:
    virtual void assign(SdScriptValuePtr src) override;
  };

#endif // SDSCRIPTVALUEVARIABLEPARAM_H
