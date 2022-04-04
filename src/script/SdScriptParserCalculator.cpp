#include "SdScriptParserCalculator.h"
#include "SdScriptValueVariableParam.h"
#include "SdScriptValueFunParam.h"

SdScriptParserCalculator::SdScriptParserCalculator(SdPItemSheet *sheet , QTableWidget *tableWidget) :
  SdScriptParser(tableWidget),
  mSheet(sheet)
  {
  addFunction( QStringLiteral("param"), [tableWidget] () -> SdScriptValueFunction* { return new SdScriptValueFunParam( tableWidget ); } );

  }


SdScriptValueVariablePtr SdScriptParserCalculator::buildParamVariable(SdScriptValue *expr)
  {
  Q_UNUSED(expr)
  return new SdScriptValueVariableParam( mSheet );
  }
