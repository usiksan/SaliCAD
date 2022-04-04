#ifndef SDSCRIPTPARSERCALCULATOR_H
#define SDSCRIPTPARSERCALCULATOR_H

#include "SdScriptParser.h"

class SdPItemSheet;

class SdScriptParserCalculator : public SdScriptParser
  {
    SdPItemSheet *mSheet; //!< Sheet which on base executed calculations
  public:
    SdScriptParserCalculator( SdPItemSheet *sheet,  QTableWidget *tableWidget );

    // SdScriptParser interface
  protected:
    virtual SdScriptValueVariablePtr buildParamVariable(SdScriptValue *expr) override;
  };

#endif // SDSCRIPTPARSERCALCULATOR_H
