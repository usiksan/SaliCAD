#ifndef SDM3DPARSER_H
#define SDM3DPARSER_H

#include "SdM3dValue.h"
#include "SdM3dOperator.h"
#include "SdM3dScaner.h"
#include "SdM3dVariable.h"
#include "SdM3dFunction.h"
#include "SdM3dProgramm.h"

#include <QMap>
#include <functional>

using SdM3dFunctionBuilder = std::function<SdM3dFunction* ()>;

class SdM3dParser
  {
    SdM3dScaner                        mScaner;
    QMap<QString,SdM3dVariablePtr>     mVaribales;
    QMap<QString,SdM3dFunctionBuilder> mFunctions;
  public:
    SdM3dParser();

    SdM3dProgramm *parse( const QString src );

    void addFunction( const QString functionName, SdM3dFunctionBuilder functionBuilder ) { mFunctions.insert( functionName, functionBuilder ); }
  private:
    SdM3dOperator *parseOperator();
    SdM3dValue    *parseExpression();
    SdM3dValue    *parseMultDiv();
    SdM3dValue    *parsePlusMinus();
    SdM3dValue    *parseMinus();
    SdM3dValue    *parseVar();
    SdM3dValue    *parseFunction( const QString &functionName );
    SdM3dValue    *failValue() const;
    //parenthesis
  };

#endif // SDM3DPARSER_H
