#ifndef SDSCRIPTPARSERCALCULATOR_H
#define SDSCRIPTPARSERCALCULATOR_H

#include "SdScriptParser.h"
#include "SdScriptRefMap.h"


class SdScriptParserCalculator : public SdScriptParser
  {
    SdScriptRefMap *mRefMap; //!< Mapping variables for visual editing
  public:
    SdScriptParserCalculator( SdScriptRefMap *refMap,  QTableWidget *tableWidget );

    // SdScriptParser interface
  protected:
    virtual SdScriptValueVariablePtr buildRefVariable( const QString &name, SdScriptValue *expr ) override;
  };

#endif // SDSCRIPTPARSERCALCULATOR_H
