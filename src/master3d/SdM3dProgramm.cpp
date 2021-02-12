#include "SdM3dProgramm.h"

SdM3dProgramm::SdM3dProgramm()
  {

  }



SdM3dProgramm::~SdM3dProgramm()
  {
  //Destroy all variables
  qDeleteAll(mVariables);
  }




void SdM3dProgramm::setVariables(SdM3dVariablePtrList varList)
  {
  qDeleteAll(mVariables);
  mVariables = varList;
  }
