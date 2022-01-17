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

  If operator execites one of two operators on condition
*/
#ifndef SDSCRIPTOPERATORIF_H
#define SDSCRIPTOPERATORIF_H

#include "SdScriptOperator.h"
#include "SdScriptValue.h"

class SdM3dOperatorIf : public SdScriptOperator
  {
    SdScriptValue    *mCondition; //!< Condition with bool result
    SdScriptOperator *mTrue;      //!< Operator executes when condition is true
    SdScriptOperator *mFalse;     //!< Operator executes when condition is false
  public:
    SdM3dOperatorIf( SdScriptValue *cond, SdScriptOperator *opTrue, SdScriptOperator *opFalse = nullptr );

    ~SdM3dOperatorIf();

    // SdM3dOperator interface
  public:
    //!
    //! \brief execute Execute operator
    //!
    virtual void execute() override;
  };

#endif // SDSCRIPTOPERATORIF_H
