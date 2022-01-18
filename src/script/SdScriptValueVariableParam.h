#ifndef SDSCRIPTVALUEVARIABLEPARAM_H
#define SDSCRIPTVALUEVARIABLEPARAM_H

#include "SdScriptValueVariable.h"

class SdPItemSheet;
class SdObject;

class SdScriptValueVariableParam : public SdScriptValueVariable
  {
    SdPItemSheet *mSheet;     //!< Sheet which on parameter works
    SdObject     *mComp;      //!< Component or net
    //QString       mRefId;     //!< Component id or net name
    QString       mParam;     //!< Parametr name
    QString       mDimension; //!< Parametr value dimension
    QString       mRow;       //!< Row definition for value ajust
    float         mValue;     //!< Value of parameter
  public:
    SdScriptValueVariableParam( SdPItemSheet *sheet, const QString &refId, const QString &param, const QString &dimension );

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
    virtual float toFloat() const override { return mValue; }

    // SdScriptValueVariable interface
  public:
    virtual void assign(SdScriptValuePtr src) override;

  private:
    float paramGet() const;
  };

#endif // SDSCRIPTVALUEVARIABLEPARAM_H
