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

  Block of operators executed consiquently as single operator
*/
#include "SdScriptOperatorBlock.h"

SdScriptOperatorBlock::SdScriptOperatorBlock()
  {

  }

SdScriptOperatorBlock::~SdScriptOperatorBlock()
  {
  qDeleteAll( mOperatorList );
  }




//!
//! \brief execute Execute operator
//!
void SdScriptOperatorBlock::execute()
  {
  for( auto ptr : qAsConst(mOperatorList) )
    ptr->execute();
  }
