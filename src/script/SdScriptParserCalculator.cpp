#include "SdScriptParserCalculator.h"
#include "SdScriptValueVariableRef.h"
#include "SdScriptValueFunRef.h"

SdScriptParserCalculator::SdScriptParserCalculator(SdGraphScript *graphScript , QTableWidget *tableWidget) :
  SdScriptParser(tableWidget),
  mGraphScript(graphScript)
  {
  addFunction( QStringLiteral("ref"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunRef(); } );

  }


SdScriptValueVariablePtr SdScriptParserCalculator::buildRefVariable(const QString &name, SdScriptValue *expr)
  {
  //In this call only function SdScriptValueFunRef will return SD_SCRIPT_TYPE_REF
  SdScriptValueFunRef *ref = dynamic_cast<SdScriptValueFunRef*>(expr);
  Q_ASSERT_X( ref != nullptr, "SdScriptParserCalculator::buildParamVariable", "Wrong expression return SD_SCRIPT_TYPE_REF" );
  return new SdScriptValueVariableRef( mGraphScript, name, ref->dimension(), ref->row() );
  }
