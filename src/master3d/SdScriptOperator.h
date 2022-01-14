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

  Base for all operators of script language
*/
#ifndef SDSCRIPTOPERATOR_H
#define SDSCRIPTOPERATOR_H


class SdScriptOperator
  {
  public:
    SdScriptOperator() {}
    virtual ~SdScriptOperator() {}

    //!
    //! \brief execute Execute operator
    //!
    virtual void execute() = 0;
  };

using SdScriptOperatorPtr = SdScriptOperator*;

#endif // SDSCRIPTOPERATOR_H
