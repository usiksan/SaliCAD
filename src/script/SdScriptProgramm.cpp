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

  Compiled programm in internal tree-code
*/
#include "SdScriptProgramm.h"

SdScriptProgramm::SdScriptProgramm()
  {

  }



SdScriptProgramm::~SdScriptProgramm()
  {
  //Destroy all variables
  qDeleteAll(mVariables);
  }




//!
//! \brief setVariables Set variables associated with this programm
//! \param varList      Variable list
//!
void SdScriptProgramm::setVariables(SdScriptVariablePtrList varList)
  {
  //Delete previously variables
  qDeleteAll(mVariables);
  //Assign new variables
  mVariables = varList;
  }




