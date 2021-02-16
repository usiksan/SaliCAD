#include "SdM3dProgramm.h"

SdM3dProgramm::SdM3dProgramm()
  {

  }



SdM3dProgramm::~SdM3dProgramm()
  {
  //Destroy all variables
  qDeleteAll(mVariables);
  }




//!
//! \brief setVariables Set variables associated with this programm
//! \param varList      Variable list
//!
void SdM3dProgramm::setVariables(SdM3dVariablePtrList varList)
  {
  //Delete previously variables
  qDeleteAll(mVariables);
  //Assign new variables
  mVariables = varList;
  }




