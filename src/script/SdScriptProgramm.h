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
#ifndef SDSCRIPTPROGRAMM_H
#define SDSCRIPTPROGRAMM_H

#include "SdScriptOperatorBlock.h"
#include "SdScriptValueVariable.h"

using SdScriptVariablePtrList = QList<SdScriptValueVariablePtr>;

class SdScriptProgramm : public SdScriptOperatorBlock
  {
    SdScriptVariablePtrList mVariables; //!< All variables for programm
  public:
    SdScriptProgramm();
    ~SdScriptProgramm();

    //!
    //! \brief setVariables Set variables associated with this programm
    //! \param varList      Variable list
    //!
    void setVariables( SdScriptVariablePtrList varList );

  };

#endif // SDSCRIPTPROGRAMM_H
