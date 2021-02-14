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

class SdPItemPart;

class SdM3dParser
  {
    SdM3dScaner                        mScaner;
    QMap<QString,SdM3dVariablePtr>     mVaribales;
    QMap<QString,SdM3dFunctionBuilder> mFunctions;
  public:
    SdM3dParser();

    SdM3dProgramm *parse( const QString src, SdPItemPart *part );

    QString error() const { return mScaner.errorGet(); }

    void    addFunction( const QString functionName, SdM3dFunctionBuilder functionBuilder ) { mFunctions.insert( functionName, functionBuilder ); }
  private:
    SdM3dOperator *parseOperator();
    SdM3dOperator *parseOperatorIf();
    SdM3dOperator *parseOperatorWhile();
    SdM3dValue    *parseExpression();
    SdM3dValue    *parseAnd();
    SdM3dValue    *parseOr();
    SdM3dValue    *parseNot();
    SdM3dValue    *parsePlusMinus();
    SdM3dValue    *parseMultDiv();
    SdM3dValue    *parseMinus();
    SdM3dValue    *parseVar();
    SdM3dValue    *parseFunction( const QString &functionName );
    SdM3dValue    *failValue() const;
    //parenthesis
  };

#endif // SDM3DPARSER_H
