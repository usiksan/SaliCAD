#ifndef SDM3DPARSER_H
#define SDM3DPARSER_H

#include "SdM3dValue.h"
#include "SdM3dOperator.h"
#include "SdM3dScaner.h"
#include "SdM3dVariable.h"
#include "SdM3dFunction.h"
#include "SdM3dProgramm.h"

#include <QMap>
#include <QTableWidget>
#include <functional>

enum {
  SDM3D_INPUT_STRING = Qt::UserRole + 1,
  SDM3D_INPUT_BOOL,
  SDM3D_INPUT_COLOR,
  SDM3D_INPUT_PIN
};


using SdM3dFunctionBuilder = std::function<SdM3dFunction* ()>;

class SdPItemPart;

class SdM3dParser
  {
    SdM3dScaner                        mScaner;
    QMap<QString,SdM3dVariablePtr>     mVariables;
    QStringList                        mVariableNameList; //!< Variable name list generated at and of parsing
    QMap<QString,SdM3dFunctionBuilder> mFunctions;
  public:
    SdM3dParser( QTableWidget *tableWidget );

    //!
    //! \brief parse Execute parsing of programm source text and generates tree of programm
    //! \param src   Programm source text
    //! \param part  Part to which will be placed generated faces
    //! \return      Programm tree
    //!
    SdM3dProgramm *parse(const QString src, SdPItemPart *part);

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
