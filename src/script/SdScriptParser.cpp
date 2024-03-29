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

#include "objects/SdObject.h"
#include "SdScriptParser.h"
#include "SdScriptOperatorBlock.h"
#include "SdScriptOperatorAssign.h"
#include "SdScriptOperatorIf.h"
#include "SdScriptOperatorWhile.h"

#include "SdScriptValueVariableBool.h"
#include "SdScriptValueVariableFloat.h"
#include "SdScriptValueVariableFloatList.h"
#include "SdScriptValueVariableColor.h"
#include "SdScriptValueVariableString.h"
#include "SdScriptValueVariableVertex.h"
#include "SdScriptValueVariableMatrix.h"
#include "SdScriptValueVariableFaceList.h"
#include "SdScriptValueVariableFace.h"
#include "SdScriptValueVariableModel.h"
#include "SdScriptValueVariableGraph.h"

#include "SdScriptValue.h"
#include "SdScriptValueFloat.h"
#include "SdScriptValueString.h"
#include "SdScriptValueBool.h"

#include "SdScriptValueArray3dVertex.h"
#include "SdScriptValueArrayFloat.h"
#include "SdScriptValueArray3dFace.h"

#include "SdScriptValueOpBinaryFloatMult.h"
#include "SdScriptValueOpBinaryFloatDiv.h"
#include "SdScriptValueOpBinaryFloatAdd.h"
#include "SdScriptValueOpBinaryFloatSub.h"
#include "SdScriptValueOpBinaryFaceListAdd.h"
#include "SdScriptValueOpUnaryFloatMinus.h"
#include "SdScriptReference.h"

#include "SdScriptValueOpBinaryBoolAnd.h"
#include "SdScriptValueOpBinaryBoolOr.h"
#include "SdScriptValueOpBinaryBoolFloatLess.h"
#include "SdScriptValueOpUnaryBoolNot.h"

//Functions
#include "SdScriptValueFunInputFloat.h"
#include "SdScriptValueFunInputColor.h"
#include "SdScriptValueFunInputPad.h"

#include "SdScriptValueFunColorBuild.h"
#include "SdScriptValueFunColorFromString.h"

SdScriptParser::SdScriptParser(QTableWidget *tableWidget)
  {
  //Fill functions
  addFunction( QStringLiteral("inputFloat"), [tableWidget] () -> SdScriptValueFunction* { return new SdScriptValueFunInputFloat( tableWidget ); },
               QStringLiteral("inputFloat( string title, float default )\nAppend row to input table for input float"));
  addFunction( QStringLiteral("inputColor"), [tableWidget] () -> SdScriptValueFunction* { return new SdScriptValueFunInputColor( tableWidget ); },
               QStringLiteral("inputColor( string title, string default )\nAppend row to input table for input color in format \"#rrggbb\"") );
  addFunction( QStringLiteral("inputPad"), [tableWidget] () -> SdScriptValueFunction* { return new SdScriptValueFunInputPad( tableWidget ); },
               QStringLiteral("inputColor( string title, string default )\nAppend row to input table for input pad description") );

  addFunction( QStringLiteral("color"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunColorBuild(); },
               QStringLiteral("color( float red, float green, float blue, float alpha )\nBuilds color from its components (0-1.0)") );
  addFunction( QStringLiteral("colorFromString"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunColorFromString(); },
               QStringLiteral("colorFromString( string color )\nConvert to color from text color description in format \"#rrggbb\"") );

  }




//!
//! \brief parse   Execute parsing of programm source text and generates tree of programm
//! \param src     Programm source text
//! \return        Programm tree
//!
SdScriptProgrammPtr SdScriptParser::parse( const QString src )
  {
  //Init scaner with programm source
  mScaner.sourceSetString( src );
  mScaner.tokenNext();

  SdScriptProgrammPtr prog( new SdScriptProgramm() );

  //Scan and append all operators of programm
  while( !mScaner.isEndOfScanOrError() ) {
    SdScriptOperator *op = parseOperator();
    if( op != nullptr )
      prog->append( op );
    }

  //Extract variables name
  mVariableNameList = mVariables.keys();

  //Append variables
  prog->setVariables( mVariables.values() );
  mVariables.clear();

  return prog;
  }




SdScriptOperator *SdScriptParser::parseOperator()
  {
  if( mScaner.matchToken('{') ) {
    //Block operator
    SdScriptOperatorBlock *block = new SdScriptOperatorBlock();
    while( !mScaner.matchToken('}') && !mScaner.isEndOfScanOrError() ) {
      SdScriptOperator *op = parseOperator();
      if( op != nullptr )
        block->append( op );
      }
    return block;
    }

  QString variableName = mScaner.tokenNeedValue('n');
  if( variableName.isEmpty() )
    return nullptr;

  //Keywords
  if( variableName == QStringLiteral("if") )
    return parseOperatorIf();

  if( variableName == QStringLiteral("while") )
    return parseOperatorWhile();

  if( variableName == QStringLiteral("for") ) {
    mScaner.error( QObject::tr("for is reserved keyword") );
    return nullptr;
    }

  if( !mScaner.tokenNeed('=', QObject::tr("Need assign =") )  )
    return nullptr;

  SdScriptValue *val = parseExpression();

  if( mVariables.contains(variableName) ) {
    SdScriptValueVariable *var = mVariables.value(variableName);
    if( var->type() != val->type() ) {
      //Illegal type
      mScaner.error( QObject::tr("Illegal type of assignment") );
      delete val;
      return nullptr;
      }

    if( var->isReadOnly() ) {
      //Impossible to assign value to read-only variable
      mScaner.error( QObject::tr("Unable to assign to read-only variable") );
      delete val;
      return nullptr;
      }

    return new SdScriptOperatorAssign( var, val );
    }

  //Depending on the type of expression, build a variable
  SdScriptValueVariable *var = nullptr;
  switch( val->type() ) {
    case SD_SCRIPT_TYPE_BOOL      : var = new SdScriptValueVariableBool(); break;
    case SD_SCRIPT_TYPE_FLOAT     : var = new SdScriptValueVariableFloat(); break;
    case SD_SCRIPT_TYPE_AFLOAT    : var = new SdScriptValueVariableFloatList(); break;
    case SD_SCRIPT_TYPE_COLOR     : var = new SdScriptValueVariableColor(); break;
    case SD_SCRIPT_TYPE_STRING    : var = new SdScriptValueVariableString(); break;
    case SD_SCRIPT_TYPE_VERTEX    : var = new SdScriptValueVariableVertex(); break;
    case SD_SCRIPT_TYPE_MATRIX    : var = new SdScriptValueVariableMatrix(); break;
    case SD_SCRIPT_TYPE_FACE_LIST : var = new SdScriptValueVariableFaceList(); break;
    case SD_SCRIPT_TYPE_FACE      : var = new SdScriptValueVariableFace(); break;
    case SD_SCRIPT_TYPE_MODEL     : var = new SdScriptValueVariableModel(); break;
    case SD_SCRIPT_TYPE_GRAPH     : var = new SdScriptValueVariableGraph(); break;
    case SD_SCRIPT_TYPE_REF       :
      var = buildRefVariable( variableName, val );
      if( var != nullptr ) break;
    default:
      mScaner.error( QObject::tr("Can't create variable with this type %1").arg(val->type()) );
      //Can't create variable
      delete val;
      return nullptr;
    }

  var->nameSet( variableName );
  mVariables.insert( variableName, var );
  return new SdScriptOperatorAssign( var, val );
  }








SdScriptOperator *SdScriptParser::parseOperatorIf()
  {
  if( !mScaner.tokenNeed('(', QObject::tr("Need assign (") )  )
    return nullptr;

  SdScriptValue *condition = parseExpression();
  if( condition->type() != SD_SCRIPT_TYPE_BOOL ) {
    mScaner.error( QObject::tr("Condition must be bool expression") );
    delete condition;
    return nullptr;
    }

  if( !mScaner.tokenNeed( ')', QObject::tr("No closing )") ) ) {
    delete condition;
    return nullptr;
    }

  SdScriptOperator *opTrue = parseOperator();
  if( opTrue == nullptr ) {
    mScaner.error( QObject::tr("Need if-else operator") );
    delete condition;
    return nullptr;
    }
  SdScriptOperator *opFalse = nullptr;

  if( mScaner.token() == 'n' && mScaner.tokenValue() == QStringLiteral("else") ) {
    mScaner.tokenNext();
    opFalse = parseOperator();
    }

  return new SdM3dOperatorIf( condition, opTrue, opFalse );
  }



SdScriptOperator *SdScriptParser::parseOperatorWhile()
  {
  if( !mScaner.tokenNeed('(', QStringLiteral("Need assign (") )  )
    return nullptr;

  SdScriptValue *condition = parseExpression();
  if( condition->type() != SD_SCRIPT_TYPE_BOOL ) {
    mScaner.error( QObject::tr("Condition must be bool expression") );
    delete condition;
    return nullptr;
    }

  if( !mScaner.tokenNeed( ')', QObject::tr("No closing )") ) ) {
    delete condition;
    return nullptr;
    }

  SdScriptOperator *opTrue = parseOperator();
  if( opTrue == nullptr ) {
    mScaner.error( QObject::tr("Need while operator") );
    delete condition;
    return nullptr;
    }

  return new SdScriptOperatorWhile( condition, opTrue );
  }




SdScriptValue *SdScriptParser::parseExpression()
  {
  return parseAnd();
  }




SdScriptValue *SdScriptParser::parseAnd()
  {
  SdScriptValue *val = parseOr();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('&') ) {
      SdScriptValue *val2 = parseOr();
      if( val->type() != SD_SCRIPT_TYPE_BOOL || val2->type() != SD_SCRIPT_TYPE_BOOL ) {
        mScaner.error( QObject::tr("Invalid types of AND operation. Both must be bool.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdScriptValueOpBinaryBoolAnd( val, val2 );
      }
    else break;
    }
  return val;
  }




SdScriptValue *SdScriptParser::parseOr()
  {
  SdScriptValue *val = parseNot();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('|') ) {
      SdScriptValue *val2 = parseNot();
      if( val->type() != SD_SCRIPT_TYPE_BOOL || val2->type() != SD_SCRIPT_TYPE_BOOL ) {
        mScaner.error( QObject::tr("Invalid types of OR operation. Both must be bool.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdScriptValueOpBinaryBoolOr( val, val2 );
      }
    else break;
    }
  return val;
  }




SdScriptValue *SdScriptParser::parseNot()
  {
  if( mScaner.matchToken( '!' ) ) {
    SdScriptValue *val = parseNot();
    if( val->type() != SD_SCRIPT_TYPE_BOOL ) {
      mScaner.error( QObject::tr("Invalid type of unary not operation. Must be bool.") );
      delete val;
      return failValue();
      }
    return new SdScriptValueOpUnaryBoolNot( val );
    }
  return parseLess();
  }





SdScriptValue *SdScriptParser::parseLess()
  {
  SdScriptValue *val = parsePlusMinus();
  if( !mScaner.isEndOfScanOrError() && mScaner.matchToken('<') ) {
    SdScriptValue *val2 = parsePlusMinus();
    if( val->type() != SD_SCRIPT_TYPE_FLOAT || val2->type() != SD_SCRIPT_TYPE_FLOAT ) {
      mScaner.error( QObject::tr("Invalid types of less operation. Both must be float.") );
      delete val;
      delete val2;
      return failValue();
      }
    return new SdScriptValueOpBinaryBoolFloatLess( val, val2 );
    }
  if( !mScaner.isEndOfScanOrError() && mScaner.matchToken('>') ) {
    SdScriptValue *val2 = parsePlusMinus();
    if( val->type() != SD_SCRIPT_TYPE_FLOAT || val2->type() != SD_SCRIPT_TYPE_FLOAT ) {
      mScaner.error( QObject::tr("Invalid types of less operation. Both must be float.") );
      delete val;
      delete val2;
      return failValue();
      }
    return new SdScriptValueOpBinaryBoolFloatLess( val2, val );
    }
  return val;
  }




SdScriptValue *SdScriptParser::parsePlusMinus()
  {
  SdScriptValue *val = parseMultDiv();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('+') ) {
      SdScriptValue *val2 = parseMultDiv();
      if( val->type() == SD_SCRIPT_TYPE_FACE_LIST && val2->type() == SD_SCRIPT_TYPE_FACE_LIST ) {
        val = new SdScriptValueOpBinaryFaceListAdd( val, val2 );
        }
      else {
        if( val->type() != SD_SCRIPT_TYPE_FLOAT || val2->type() != SD_SCRIPT_TYPE_FLOAT ) {
          mScaner.error( QObject::tr("Invalid types of add operation. Both must be float.") );
          delete val;
          delete val2;
          return failValue();
          }
        val = new SdScriptValueOpBinaryFloatAdd( val, val2 );
        }
      }
    else if( mScaner.matchToken('-') ) {
      SdScriptValue *val2 = parseMultDiv();
      if( val->type() != SD_SCRIPT_TYPE_FLOAT || val2->type() != SD_SCRIPT_TYPE_FLOAT ) {
        mScaner.error( QStringLiteral("Invalid types of sub operation. Both must be float.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdScriptValueOpBinaryFloatSub( val, val2 );
      }
    else break;
    }
  return val;
  }





SdScriptValue *SdScriptParser::parseMultDiv()
  {
  SdScriptValue *val = parseMinus();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('*') ) {
      SdScriptValue *val2 = parseMinus();
      if( val->type() != SD_SCRIPT_TYPE_FLOAT || val2->type() != SD_SCRIPT_TYPE_FLOAT ) {
        mScaner.error( QObject::tr("Invalid types of multiply operation. Both must be float.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdScriptValueOpBinaryFloatMult( val, val2 );
      }
    else if( mScaner.matchToken('/') ) {
      SdScriptValue *val2 = parseMinus();
      if( val->type() != SD_SCRIPT_TYPE_FLOAT || val2->type() != SD_SCRIPT_TYPE_FLOAT ) {
        mScaner.error( QObject::tr("Invalid types of divide operation. Both must be float.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdScriptValueOpBinaryFloatDiv( val, val2 );
      }
    else break;
    }
  return val;
  }





SdScriptValue *SdScriptParser::parseMinus()
  {
  if( mScaner.matchToken( '-' ) ) {
    SdScriptValue *val = parseMinus();
    if( val->type() != SD_SCRIPT_TYPE_FLOAT ) {
      mScaner.error( QObject::tr("Invalid type of unary minus operation. Must be float.") );
      delete val;
      return failValue();
      }
    return new SdScriptValueOpUnaryFloatMinus( val );
    }
  return parseVar();
  }







SdScriptValue *SdScriptParser::parseVar()
  {
  if( mScaner.token() == 'n' ) {
    //Variable or function
    QString name = mScaner.tokenValue();
    mScaner.tokenNext();

    if( name == QStringLiteral("true") )
      return new SdScriptValueBool( true );
    if( name == QStringLiteral("false") )
      return new SdScriptValueBool( false );

    if( mScaner.matchToken('(') )
      return parseFunction( name );

    if( mVariables.contains(name) ) {
      auto var = mVariables.value(name);
      if( var->isWriteOnly() ) {
        mScaner.error( QObject::tr("Unable read write-only variable '%1'").arg(name) );
        return failValue();
        }
      return new SdScriptReference( var );
      }

    mScaner.error( QObject::tr("Undefined variable '%1'").arg(name) );
    return failValue();
    }

  else if( mScaner.token() == 'd' ) {
    //float
    SdScriptValue *val = new SdScriptValueFloat( mScaner.tokenValueFloat() );
    mScaner.tokenNext();
    return val;
    }

  else if( mScaner.token() == 's' ) {
    //String
    SdScriptValue *val = new SdScriptValueString( mScaner.tokenValue() );
    mScaner.tokenNext();
    return val;
    }

  else if( mScaner.matchToken( '[' ) ) {
    //list object
    // [ 1, 2, 3, 4 ]
    SdScriptValue *val = parseExpression();
    SdScriptValueArray *array;
    if( val->type() == SD_SCRIPT_TYPE_VERTEX )
      array = new SdScriptValueArray3dVertex();
    else if( val->type() == SD_SCRIPT_TYPE_FLOAT )
      array = new SdScriptValueArrayFloat();
    else if( val->type() == SD_SCRIPT_TYPE_FACE )
      array = new SdScriptValueArray3dFace();
    else {
      mScaner.error( QObject::tr("Invalid element type of array. Must be vertex, segment or face") );
      delete val;
      return failValue();
      }

    array->append( val );
    while( mScaner.matchToken( ',' )  ) {
      val = parseExpression();
      if( val->type() == array->typeOfElement() || val->type() == array->type() )
        array->append( val );
      else {
        mScaner.error( QObject::tr("Illegal type of array element") );
        delete val;
        break;
        }
      }
    mScaner.tokenNeed( ']', QObject::tr("No closing ]") );
    return array;
    }

  else if( mScaner.matchToken( '(' ) ) {
    // ( expr )
    SdScriptValue *val = parseExpression();
    mScaner.tokenNeed( ')', QObject::tr("No closing )") );
    return val;
    }

  return failValue();
  }




SdScriptValue *SdScriptParser::parseFunction(const QString &functionName)
  {
  if( mFunctions.contains(functionName) ) {
    SdScriptValueFunction *function = mFunctions.value(functionName) ();
    for( int i = 0; i < function->paramCount(); i++ ) {
      SdScriptValue *param = parseExpression();
      if( param->type() != function->paramType(i) ) {
        mScaner.error( QObject::tr("Invalid function param %1").arg(i) );
        delete param;
        delete function;
        return failValue();
        }
      function->paramSet( i, param );

      if( i + 1 < function->paramCount() ) {
        if( !mScaner.tokenNeed( ',', QObject::tr("Need comma") ) ) {
          delete function;
          return failValue();
          }
        }
      else {
        if( !mScaner.tokenNeed( ')', QObject::tr("Need closing )") ) ) {
          delete function;
          return failValue();
          }
        }
      }
    return function;
    }

  mScaner.error( QObject::tr("No function '%1'").arg(functionName) );
  return failValue();
  }




SdScriptValue *SdScriptParser::failValue() const
  {
  return new SdScriptValue();
  }


