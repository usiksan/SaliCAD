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

  Operator assignment of expression
*/
#ifndef SDSCRIPTOPERATORASSIGN_H
#define SDSCRIPTOPERATORASSIGN_H

#include "SdScriptOperator.h"
#include "SdScriptValueVariable.h"

class SdScriptOperatorAssign : public SdScriptOperator
  {
    SdScriptValueVariable *mVariable; //!< Variable to which will be assigned value calculated from expression
    SdScriptValue    *mValue; //!< Expression
  public:
    SdScriptOperatorAssign( SdScriptValueVariable *var, SdScriptValue *val ) : mVariable(var), mValue(val) {}
    ~SdScriptOperatorAssign() { delete mValue; }

    // SdM3dOperator interface
  public:
    //!
    //! \brief execute Execute operator
    //!
    virtual void execute() override { mVariable->assign( mValue ); }
  };

#endif // SDSCRIPTOPERATORASSIGN_H
