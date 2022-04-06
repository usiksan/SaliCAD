#ifndef SDSCRIPTPARSERCALCULATOR_H
#define SDSCRIPTPARSERCALCULATOR_H

#include "SdScriptParser.h"

class SdGraphScript;

class SdScriptParserCalculator : public SdScriptParser
  {
    SdGraphScript *mGraphScript; //!< Script object in graphics editor
  public:
    SdScriptParserCalculator( SdGraphScript *graphScript,  QTableWidget *tableWidget );

    // SdScriptParser interface
  protected:
    virtual SdScriptValueVariablePtr buildRefVariable( const QString &name, SdScriptValue *expr ) override;
  };

#endif // SDSCRIPTPARSERCALCULATOR_H
