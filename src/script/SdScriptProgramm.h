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

#include <QSharedPointer>

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

    //!
    //! \brief variable Find and return variable by its name
    //! \param name     Name of variable to find
    //! \return         Found variable or nullptr if not found
    //!
    SdScriptValueVariable *variable( const QString &name ) const;

  };

using SdScriptProgrammPtr = QSharedPointer<SdScriptProgramm>;

#endif // SDSCRIPTPROGRAMM_H
