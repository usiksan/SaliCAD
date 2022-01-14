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
#include "SdScriptOperatorIf.h"

SdM3dOperatorIf::SdM3dOperatorIf( SdScriptValue *cond, SdScriptOperator *opTrue, SdScriptOperator *opFalse ) :
  mCondition(cond),
  mTrue(opTrue),
  mFalse(opFalse)
  {

  }


SdM3dOperatorIf::~SdM3dOperatorIf()
  {
  delete mCondition;
  delete mTrue;
  if( mFalse != nullptr )
    delete mFalse;
  }





//!
//! \brief execute Execute operator
//!
void SdM3dOperatorIf::execute()
  {
  if( mCondition->toBool() )
    mTrue->execute();
  else if( mFalse != nullptr )
    mFalse->execute();
  }
