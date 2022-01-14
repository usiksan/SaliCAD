#ifndef SDM3DPARSER_H
#define SDM3DPARSER_H

#include "SdScriptValue.h"
#include "SdScriptOperator.h"
#include "SdScriptScaner.h"
#include "SdScriptValueVariable.h"
#include "SdScriptValueFunction.h"
#include "SdScriptProgramm.h"

#include <QMap>
#include <QTableWidget>
#include <functional>

enum {
  SDM3D_INPUT_STRING = Qt::UserRole + 1,
  SDM3D_INPUT_BOOL,
  SDM3D_INPUT_COLOR,
  SDM3D_INPUT_PAD
};


using SdM3dFunctionBuilder = std::function<SdScriptValueFunction* ()>;

class SdPItemPart;

class SdM3dParser
  {
    SdScriptScaner                        mScaner;
    QMap<QString,SdScriptValueVariablePtr>     mVariables;        //!< Variables map. Associates variable name with variable
    QStringList                        mVariableNameList; //!< Variable name list generated at and of parsing
    QMap<QString,SdM3dFunctionBuilder> mFunctions;        //!< Predefined functions
  public:
    SdM3dParser( QTableWidget *tableWidget );

    //!
    //! \brief parse   Execute parsing of programm source text and generates tree of programm
    //! \param src     Programm source text
    //! \param part    Part to which will be placed generated faces
    //! \return        Programm tree
    //!
    SdScriptProgramm *parse(const QString src, SdPItemPart *part);

    //!
    //! \brief variableNameList Returns parsed programm variable name list
    //! \return                 Variable name list
    //!
    QStringList    variableNameList() const { return mVariableNameList; }

    //!
    //! \brief functionNameList Returns defined function name list
    //! \return                 Function name list
    //!
    QStringList    functionNameList() const { return mFunctions.keys(); }

    QString        error() const { return mScaner.errorGet(); }

    void           addFunction( const QString functionName, SdM3dFunctionBuilder functionBuilder ) { mFunctions.insert( functionName, functionBuilder ); }
  private:
    SdScriptOperator *parseOperator();
    SdScriptOperator *parseOperatorIf();
    SdScriptOperator *parseOperatorWhile();
    SdScriptValue    *parseExpression();
    SdScriptValue    *parseAnd();
    SdScriptValue    *parseOr();
    SdScriptValue    *parseNot();
    SdScriptValue    *parseLess();
    SdScriptValue    *parsePlusMinus();
    SdScriptValue    *parseMultDiv();
    SdScriptValue    *parseMinus();
    SdScriptValue    *parseVar();
    SdScriptValue    *parseFunction( const QString &functionName );
    SdScriptValue    *failValue() const;
    //parenthesis
  };

#endif // SDM3DPARSER_H
