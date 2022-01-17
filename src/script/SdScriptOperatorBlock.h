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
#ifndef SDSCRIPTOPERATORBLOCK_H
#define SDSCRIPTOPERATORBLOCK_H

#include "SdScriptOperator.h"

#include <QList>

class SdScriptOperatorBlock : public SdScriptOperator
  {
    QList<SdScriptOperatorPtr> mOperatorList; //!< List of operators
  public:
    SdScriptOperatorBlock();
    ~SdScriptOperatorBlock();

    //!
    //! \brief append Append operator to block list
    //! \param op     Appended operator
    //!
    void append( SdScriptOperatorPtr op ) { mOperatorList.append( op ); }


    // SdM3dOperator interface
  public:
    //!
    //! \brief execute Execute operator
    //!
    virtual void execute() override;
  };

#endif // SDSCRIPTOPERATORBLOCK_H
