#ifndef SDSCRIPTPARSERCALCULATOR_H
#define SDSCRIPTPARSERCALCULATOR_H

#include "SdScriptParser.h"

class SDPItemSheet;

class SdScriptParserCalculator : public SdScriptParser
  {
    SDPItemSheet *mSheet; //!< Sheet which on base executed calculations
  public:
    SdScriptParserCalculator();
  };

#endif // SDSCRIPTPARSERCALCULATOR_H
