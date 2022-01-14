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

  While operator execites operator while condition is true
*/
#ifndef SDSCRIPTOPERATORWHILE_H
#define SDSCRIPTOPERATORWHILE_H

#include "SdScriptOperator.h"
#include "SdScriptValue.h"

class SdScriptOperatorWhile : public SdScriptOperator
  {
    SdScriptValue    *mCondition; //!< Condition, when it true operator is executed
    SdScriptOperator *mBlock;     //!< Operator to execute
  public:
    SdScriptOperatorWhile( SdScriptValue *cond, SdScriptOperator *block );
    ~SdScriptOperatorWhile();

    // SdM3dOperator interface
  public:
    //!
    //! \brief execute Execute operator
    //!
    virtual void execute() override;
  };

#endif // SDSCRIPTOPERATORWHILE_H
