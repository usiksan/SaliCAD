/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Parser which scan and parse textual representative of source code of script and
  convert it to execute available tree of operations
*/
#ifndef SDSCRIPTPARSER_H
#define SDSCRIPTPARSER_H

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


using SdScriptFunctionBuilder = std::function<SdScriptValueFunction* ()>;

class SdPItemPart;

class SdScriptParser
  {
  protected:
    SdScriptScaner                         mScaner;           //!< Scaner for source code
    QMap<QString,SdScriptValueVariablePtr> mVariables;        //!< Variables map. Associates variable name with variable
    QStringList                            mVariableNameList; //!< Variable name list generated at and of parsing
    QMap<QString,SdScriptFunctionBuilder>  mFunctions;        //!< Predefined functions
  public:
    SdScriptParser( QTableWidget *tableWidget );

    //!
    //! \brief parse   Execute parsing of programm source text and generates tree of programm
    //! \param src     Programm source text
    //! \return        Programm tree
    //!
    SdScriptProgrammPtr parse(const QString src);

    //!
    //! \brief variableNameList Returns parsed programm variable name list
    //! \return                 Variable name list
    //!
    QStringList         variableNameList() const { return mVariableNameList; }

    //!
    //! \brief functionNameList Returns defined function name list
    //! \return                 Function name list
    //!
    QStringList         functionNameList() const { return mFunctions.keys(); }

    //!
    //! \brief error Return error string if any
    //! \return      Error string
    //!
    QString             error() const { return mScaner.errorGet(); }

    //!
    //! \brief addFunction     Append predefined function
    //! \param functionName    Function name
    //! \param functionBuilder Function builder
    //!
    void                addFunction( const QString functionName, SdScriptFunctionBuilder functionBuilder ) { mFunctions.insert( functionName, functionBuilder ); }

  protected:
    virtual SdScriptValueVariablePtr buildParamVariable( SdScriptValue *expr ) { Q_UNUSED(expr) return nullptr; }
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

#endif // SDSCRIPTPARSER_H
