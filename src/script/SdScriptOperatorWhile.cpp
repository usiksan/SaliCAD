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
#include "SdScriptOperatorWhile.h"

#include <QDebug>

SdScriptOperatorWhile::SdScriptOperatorWhile(SdScriptValue *cond, SdScriptOperator *block) :
  mCondition(cond),
  mBlock(block)
  {

  }

SdScriptOperatorWhile::~SdScriptOperatorWhile()
  {
  delete mCondition;
  delete mBlock;
  }




//!
//! \brief execute Execute operator
//!
void SdScriptOperatorWhile::execute()
  {
  //Exclude infinite loop. We limit it with 1000 iterations
  for( int i = 0; i < 1000 && mCondition->toBool(); i++ ) {
    mBlock->execute();
    qDebug() << "Iteration " << i;
    }
  }
