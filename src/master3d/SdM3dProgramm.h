#ifndef SDM3DPROGRAMM_H
#define SDM3DPROGRAMM_H

#include "SdM3dOperatorBlock.h"
#include "SdM3dVariable.h"

using SdM3dVariablePtrList = QList<SdM3dVariablePtr>;

class SdM3dProgramm : public SdM3dOperatorBlock
  {
    SdM3dVariablePtrList mVariables;
  public:
    SdM3dProgramm();
    ~SdM3dProgramm();

    //!
    //! \brief setVariables Set variables associated with this programm
    //! \param varList      Variable list
    //!
    void setVariables( SdM3dVariablePtrList varList );

  };

#endif // SDM3DPROGRAMM_H
