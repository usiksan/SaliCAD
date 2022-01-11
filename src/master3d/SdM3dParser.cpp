
#include "SdM3dParser.h"
#include "SdM3dOperatorBlock.h"
#include "SdM3dOperatorAssign.h"
#include "SdM3dOperatorIf.h"
#include "SdM3dOperatorWhile.h"

#include "SdM3dVariableBool.h"
#include "SdM3dVariableFloat.h"
#include "SdM3dVariableColor.h"
#include "SdM3dVariableString.h"
#include "SdM3dVariableVertex.h"
#include "SdM3dVariableMatrix.h"
#include "SdM3dVariableSegment.h"
#include "SdM3dVariablePath.h"
#include "SdM3dVariableRegion.h"
#include "SdM3dVariableFace.h"
#include "SdM3dVariableModel.h"
#include "SdM3dVariableGraph.h"

//Predefined variable to insert faces into part
#include "SdM3dPartModel.h"
//Predefined variable to insert 2d graphics into part
#include "SdM3dPartGraph.h"

#include "SdScriptValue.h"
#include "SdM3dFloat.h"
#include "SdM3dString.h"
#include "SdM3dBool.h"

#include "SdScriptValueArray3dVertex.h"
#include "SdScriptValueArray3dSegment.h"
#include "SdScriptValueArray3dFace.h"

#include "SdM3dBinaryFloatMult.h"
#include "SdM3dBinaryFloatDiv.h"
#include "SdM3dBinaryFloatAdd.h"
#include "SdM3dBinaryFloatSub.h"
#include "SdM3dUnaryFloatMinus.h"
#include "SdM3dReference.h"

#include "SdScriptValueOpBinaryBoolAnd.h"
#include "SdScriptValueOpBinaryBoolOr.h"
#include "SdScriptValueOpBinaryBoolFloatLess.h"
#include "SdM3dUnaryBoolNot.h"

//Functions
#include "SdM3dFunInputFloat.h"
#include "SdM3dFunInputColor.h"
#include "SdM3dFunInputPad.h"

#include "SdM3dFunColorBuild.h"
#include "SdM3dFunColorFromString.h"

#include "SdM3dFunStringPadRectThrou.h"
#include "SdM3dFunStringPadCircleThrou.h"
#include "SdM3dFunStringPinIndex.h"
#include "SdM3dFunStringPinMatrix.h"

#include "SdM3dFunVertexBuild.h"
#include "SdM3dFunVertexOffset.h"
#include "SdM3dFunVertexTranslate.h"
#include "SdM3dFunVertexCenterOfRegion.h"

#include "SdM3dFunRegionRect.h"
#include "SdM3dFunRegionCircle.h"
#include "SdM3dFunRegionTranslate.h"

#include "SdM3dFunFaceBuild.h"
#include "SdM3dFunFaceTranslate.h"
#include "SdM3dFunFaceTriangle.h"

#include "SdM3dFunModelWall.h"
#include "SdM3dFunModelWallEven.h"
#include "SdM3dFunModelExtrude.h"
#include "SdM3dFunModelBox.h"
#include "SdM3dFunModelCylinder.h"
#include "SdM3dFunModelTranslate.h"
#include "SdM3dFunModelPinTqfp.h"
#include "SdM3dFunModelHexagon.h"
#include "SdM3dFunModelCopy.h"
#include "SdM3dFunModelBodyBeveled.h"

#include "SdM3dFunGraphLine.h"
#include "SdM3dFunGraphCircle.h"
#include "SdM3dFunGraphRect.h"
#include "SdM3dFunGraphRectFilled.h"
#include "SdM3dFunGraphPin.h"

SdM3dParser::SdM3dParser(QTableWidget *tableWidget)
  {
  //Fill functions
  addFunction( QStringLiteral("inputFloat"), [tableWidget] () -> SdM3dFunction* { return new SdM3dFunInputFloat( tableWidget ); } );
  addFunction( QStringLiteral("inputColor"), [tableWidget] () -> SdM3dFunction* { return new SdM3dFunInputColor( tableWidget ); } );
  addFunction( QStringLiteral("inputPad"), [tableWidget] () -> SdM3dFunction* { return new SdM3dFunInputPad( tableWidget ); } );

  addFunction( QStringLiteral("color"), [] () -> SdM3dFunction* { return new SdM3dFunColorBuild(); } );
  addFunction( QStringLiteral("colorFromString"), [] () -> SdM3dFunction* { return new SdM3dFunColorFromString(); } );

  addFunction( QStringLiteral("stringPadRectThrough"), [] () -> SdM3dFunction* { return new SdM3dFunStringPadRectThrou(); } );
  addFunction( QStringLiteral("stringPadCircleThrough"), [] () -> SdM3dFunction* { return new SdM3dFunStringPadCircleThrou(); } );
  addFunction( QStringLiteral("stringPinIndex"), [] () -> SdM3dFunction* { return new SdM3dFunStringPinIndex(); } );
  addFunction( QStringLiteral("stringPinMatrix"), [] () -> SdM3dFunction* { return new SdM3dFunStringPinMatrix(); } );

  addFunction( QStringLiteral("vertex"), [] () -> SdM3dFunction* { return new SdM3dFunVertexBuild(); } );
  addFunction( QStringLiteral("vertexOffset"), [] () -> SdM3dFunction* { return new SdM3dFunVertexOffset(); } );
  addFunction( QStringLiteral("vertexTranslate"), [] () -> SdM3dFunction* { return new SdM3dFunVertexTranslate(); } );
  addFunction( QStringLiteral("vertexCenterOfRegion"), [] () -> SdM3dFunction* { return new SdM3dFunVertexCenterOfRegion(); } );

  addFunction( QStringLiteral("regionRect"), [] () -> SdM3dFunction* { return new SdM3dFunRegionRect(); } );
  addFunction( QStringLiteral("regionCircle"), [] () -> SdM3dFunction* { return new SdM3dFunRegionCircle(); } );
  addFunction( QStringLiteral("regionTranslate"), [] () -> SdM3dFunction* { return new SdM3dFunRegionTranslate(); } );

  addFunction( QStringLiteral("face"), [] () -> SdM3dFunction* { return new SdM3dFunFaceBuild(); } );
  addFunction( QStringLiteral("faceTranslate"), [] () -> SdM3dFunction* { return new SdM3dFunFaceTranslate(); } );
  addFunction( QStringLiteral("faceTriangle"), [] () -> SdM3dFunction* { return new SdM3dFunFaceTriangle(); } );

  addFunction( QStringLiteral("modelWall"), [] () -> SdM3dFunction* { return new SdM3dFunModelWall(); } );
  addFunction( QStringLiteral("modelWallEven"), [] () -> SdM3dFunction* { return new SdM3dFunModelWallEven(); } );
  addFunction( QStringLiteral("modelExtrude"), [] () -> SdM3dFunction* { return new SdM3dFunModelExtrude(); } );
  addFunction( QStringLiteral("modelBox"), [] () -> SdM3dFunction* { return new SdM3dFunModelBox(); } );
  addFunction( QStringLiteral("modelCylinder"), [] () -> SdM3dFunction* { return new SdM3dFunModelCylinder(); } );
  addFunction( QStringLiteral("modelTranslate"), [] () -> SdM3dFunction* { return new SdM3dFunModelTranslate(); } );
  addFunction( QStringLiteral("modelPinTqfp"), [] () -> SdM3dFunction* { return new SdM3dFunModelPinTqfp(); } );
  addFunction( QStringLiteral("modelHexagon"), [] () -> SdM3dFunction* { return new SdM3dFunModelHexagon(); } );
  addFunction( QStringLiteral("modelCopy"), [] () -> SdM3dFunction* { return new SdM3dFunModelCopy(); } );
  addFunction( QStringLiteral("modelBodyBeveled"), [] () -> SdM3dFunction* { return new SdM3dFunModelBodyBeveled(); } );

  addFunction( QStringLiteral("graphLine"), [] () -> SdM3dFunction* { return new SdM3dFunGraphLine(); } );
  addFunction( QStringLiteral("graphCircle"), [] () -> SdM3dFunction* { return new SdM3dFunGraphCircle(); } );
  addFunction( QStringLiteral("graphRect"), [] () -> SdM3dFunction* { return new SdM3dFunGraphRect(); } );
  addFunction( QStringLiteral("graphRectFilled"), [] () -> SdM3dFunction* { return new SdM3dFunGraphRectFilled(); } );
  addFunction( QStringLiteral("graphPin"), [] () -> SdM3dFunction* { return new SdM3dFunGraphPin(); } );

  }




//!
//! \brief parse   Execute parsing of programm source text and generates tree of programm
//! \param src     Programm source text
//! \param part    Part to which will be placed generated faces
//! \return        Programm tree
//!
SdM3dProgramm *SdM3dParser::parse(const QString src, SdPItemPart *part )
  {
  //Insert predefined variables
  mVariables.insert( QStringLiteral("partModel"), new SdM3dPartModel( part ) );
  mVariables.insert( QStringLiteral("partFlat"), new SdM3dPartGraph( part ) );

  //Init scaner with programm source
  mScaner.sourceSetString( src );
  mScaner.tokenNext();

  SdM3dProgramm *prog = new SdM3dProgramm();

  //Scan and append all operators of programm
  while( !mScaner.isEndOfScanOrError() ) {
    SdM3dOperator *op = parseOperator();
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




SdM3dOperator *SdM3dParser::parseOperator()
  {
  if( mScaner.matchToken('{') ) {
    //Block operator
    SdM3dOperatorBlock *block = new SdM3dOperatorBlock();
    while( !mScaner.matchToken('}') && !mScaner.isEndOfScanOrError() ) {
      SdM3dOperator *op = parseOperator();
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
    mScaner.error( QStringLiteral("Reserved keyword") );
    return nullptr;
    }

  if( !mScaner.tokenNeed('=', QStringLiteral("Need assign =") )  )
    return nullptr;

  SdScriptValue *val = parseExpression();

  if( mVariables.contains(variableName) ) {
    SdM3dVariable *var = mVariables.value(variableName);
    if( var->type() != val->type() ) {
      //Illegal type
      mScaner.error( QStringLiteral("Illegal type of assignment") );
      delete val;
      return nullptr;
      }

    return new SdM3dOperatorAssign( var, val );
    }

  //Depending on the type of expression, build a variable
  SdM3dVariable *var = nullptr;
  switch( val->type() ) {
    case SD_SCRIPT_TYPE_BOOL    : var = new SdM3dVariableBool(); break;
    case SD_SCRIPT_TYPE_FLOAT   : var = new SdM3dVariableFloat(); break;
    case SD_SCRIPT_TYPE_COLOR   : var = new SdM3dVariableColor(); break;
    case SD_SCRIPT_TYPE_STRING  : var = new SdM3dVariableString(); break;
    case SD_SCRIPT_TYPE_VERTEX  : var = new SdM3dVariableVertex(); break;
    case SD_SCRIPT_TYPE_MATRIX  : var = new SdM3dVariableMatrix(); break;
    case SD_SCRIPT_TYPE_SEGMENT : var = new SdM3dVariableSegment(); break;
    case SD_SCRIPT_TYPE_PATH    : var = new SdM3dVariablePath(); break;
    case SD_SCRIPT_TYPE_REGION  : var = new SdM3dVariableRegion(); break;
    case SD_SCRIPT_TYPE_FACE    : var = new SdM3dVariableFace(); break;
    case SD_SCRIPT_TYPE_MODEL   : var = new SdM3dVariableModel(); break;
    case SD_SCRIPT_TYPE_GRAPH   : var = new SdM3dVariableGraph(); break;
    default:
      mScaner.error( QStringLiteral("Can't create variable with this type %1").arg(val->type()) );
      //Can't create variable
      delete val;
      return nullptr;
    }

  mVariables.insert( variableName, var );
  return new SdM3dOperatorAssign( var, val );
  }

SdM3dOperator *SdM3dParser::parseOperatorIf()
  {
  if( !mScaner.tokenNeed('(', QStringLiteral("Need assign (") )  )
    return nullptr;

  SdScriptValue *condition = parseExpression();
  if( condition->type() != SD_SCRIPT_TYPE_BOOL ) {
    mScaner.error( "Condition must be bool expression" );
    delete condition;
    return nullptr;
    }

  if( !mScaner.tokenNeed( ')', QStringLiteral("No closing )") ) ) {
    delete condition;
    return nullptr;
    }

  SdM3dOperator *opTrue = parseOperator();
  if( opTrue == nullptr ) {
    mScaner.error( QStringLiteral("Need if-else operator") );
    delete condition;
    return nullptr;
    }
  SdM3dOperator *opFalse = nullptr;

  if( mScaner.token() == 'n' && mScaner.tokenValue() == QStringLiteral("else") ) {
    mScaner.tokenNext();
    opFalse = parseOperator();
    }

  return new SdM3dOperatorIf( condition, opTrue, opFalse );
  }



SdM3dOperator *SdM3dParser::parseOperatorWhile()
  {
  if( !mScaner.tokenNeed('(', QStringLiteral("Need assign (") )  )
    return nullptr;

  SdScriptValue *condition = parseExpression();
  if( condition->type() != SD_SCRIPT_TYPE_BOOL ) {
    mScaner.error( "Condition must be bool expression" );
    delete condition;
    return nullptr;
    }

  if( !mScaner.tokenNeed( ')', QStringLiteral("No closing )") ) ) {
    delete condition;
    return nullptr;
    }

  SdM3dOperator *opTrue = parseOperator();
  if( opTrue == nullptr ) {
    mScaner.error( QStringLiteral("Need while operator") );
    delete condition;
    return nullptr;
    }

  return new SdM3dOperatorWhile( condition, opTrue );
  }




SdScriptValue *SdM3dParser::parseExpression()
  {
  return parseAnd();
  }




SdScriptValue *SdM3dParser::parseAnd()
  {
  SdScriptValue *val = parseOr();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('&') ) {
      SdScriptValue *val2 = parseOr();
      if( val->type() != SD_SCRIPT_TYPE_BOOL || val2->type() != SD_SCRIPT_TYPE_BOOL ) {
        mScaner.error( QStringLiteral("Invalid types of AND operation. Both must be bool.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdM3dBinaryBoolAnd( val, val2 );
      }
    else break;
    }
  return val;
  }




SdScriptValue *SdM3dParser::parseOr()
  {
  SdScriptValue *val = parseNot();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('|') ) {
      SdScriptValue *val2 = parseNot();
      if( val->type() != SD_SCRIPT_TYPE_BOOL || val2->type() != SD_SCRIPT_TYPE_BOOL ) {
        mScaner.error( QStringLiteral("Invalid types of OR operation. Both must be bool.") );
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




SdScriptValue *SdM3dParser::parseNot()
  {
  if( mScaner.matchToken( '!' ) ) {
    SdScriptValue *val = parseNot();
    if( val->type() != SD_SCRIPT_TYPE_BOOL ) {
      mScaner.error( QStringLiteral("Invalid type of unary not operation. Must be bool.") );
      delete val;
      return failValue();
      }
    return new SdM3dUnaryBoolNot( val );
    }
  return parseLess();
  }





SdScriptValue *SdM3dParser::parseLess()
  {
  SdScriptValue *val = parsePlusMinus();
  if( !mScaner.isEndOfScanOrError() && mScaner.matchToken('<') ) {
    SdScriptValue *val2 = parsePlusMinus();
    if( val->type() != SD_SCRIPT_TYPE_FLOAT || val2->type() != SD_SCRIPT_TYPE_FLOAT ) {
      mScaner.error( QStringLiteral("Invalid types of less operation. Both must be float.") );
      delete val;
      delete val2;
      return failValue();
      }
    return new SdScriptValueOpBinaryBoolFloatLess( val, val2 );
    }
  if( !mScaner.isEndOfScanOrError() && mScaner.matchToken('>') ) {
    SdScriptValue *val2 = parsePlusMinus();
    if( val->type() != SD_SCRIPT_TYPE_FLOAT || val2->type() != SD_SCRIPT_TYPE_FLOAT ) {
      mScaner.error( QStringLiteral("Invalid types of less operation. Both must be float.") );
      delete val;
      delete val2;
      return failValue();
      }
    return new SdScriptValueOpBinaryBoolFloatLess( val2, val );
    }
  return val;
  }




SdScriptValue *SdM3dParser::parsePlusMinus()
  {
  SdScriptValue *val = parseMultDiv();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('+') ) {
      SdScriptValue *val2 = parseMultDiv();
      if( val->type() != SD_SCRIPT_TYPE_FLOAT || val2->type() != SD_SCRIPT_TYPE_FLOAT ) {
        mScaner.error( QStringLiteral("Invalid types of add operation. Both must be float.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdM3dBinaryFloatAdd( val, val2 );
      }
    else if( mScaner.matchToken('-') ) {
      SdScriptValue *val2 = parseMultDiv();
      if( val->type() != SD_SCRIPT_TYPE_FLOAT || val2->type() != SD_SCRIPT_TYPE_FLOAT ) {
        mScaner.error( QStringLiteral("Invalid types of sub operation. Both must be float.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdM3dBinaryFloatSub( val, val2 );
      }
    else break;
    }
  return val;
  }





SdScriptValue *SdM3dParser::parseMultDiv()
  {
  SdScriptValue *val = parseMinus();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('*') ) {
      SdScriptValue *val2 = parseMinus();
      if( val->type() != SD_SCRIPT_TYPE_FLOAT || val2->type() != SD_SCRIPT_TYPE_FLOAT ) {
        mScaner.error( QStringLiteral("Invalid types of multiply operation. Both must be float.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdM3dBinaryFloatMult( val, val2 );
      }
    else if( mScaner.matchToken('/') ) {
      SdScriptValue *val2 = parseMinus();
      if( val->type() != SD_SCRIPT_TYPE_FLOAT || val2->type() != SD_SCRIPT_TYPE_FLOAT ) {
        mScaner.error( QStringLiteral("Invalid types of divide operation. Both must be float.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdM3dBinaryFloatDiv( val, val2 );
      }
    else break;
    }
  return val;
  }





SdScriptValue *SdM3dParser::parseMinus()
  {
  if( mScaner.matchToken( '-' ) ) {
    SdScriptValue *val = parseMinus();
    if( val->type() != SD_SCRIPT_TYPE_FLOAT ) {
      mScaner.error( QStringLiteral("Invalid type of unary minus operation. Must be float.") );
      delete val;
      return failValue();
      }
    return new SdM3dUnaryFloatMinus( val );
    }
  return parseVar();
  }







SdScriptValue *SdM3dParser::parseVar()
  {
  if( mScaner.token() == 'n' ) {
    //Variable or function
    QString name = mScaner.tokenValue();
    mScaner.tokenNext();

    if( name == QStringLiteral("true") )
      return new SdM3dBool( true );
    if( name == QStringLiteral("false") )
      return new SdM3dBool( false );

    if( mScaner.matchToken('(') )
      return parseFunction( name );

    if( mVariables.contains(name) )
      return new SdM3dReference( mVariables.value(name) );

    mScaner.error( QStringLiteral("Undefined variable '%1'").arg(name) );
    return failValue();
    }

  else if( mScaner.token() == 'd' ) {
    //float
    SdScriptValue *val = new SdM3dFloat( mScaner.tokenValueFloat() );
    mScaner.tokenNext();
    return val;
    }

  else if( mScaner.token() == 's' ) {
    //String
    SdScriptValue *val = new SdM3dString( mScaner.tokenValue() );
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
    else if( val->type() == SD_SCRIPT_TYPE_SEGMENT )
      array = new SdScriptValueArray3dSegment();
    else if( val->type() == SD_SCRIPT_TYPE_FACE )
      array = new SdScriptValueArray3dFace();
    else {
      mScaner.error( QStringLiteral("Invalid element type of array. Must be vertex, segment or face") );
      delete val;
      return failValue();
      }

    array->append( val );
    while( mScaner.matchToken( ',' )  ) {
      val = parseExpression();
      if( val->type() == array->typeOfElement() )
        array->append( val );
      else {
        mScaner.error( QStringLiteral("Illegal type of array element") );
        delete val;
        break;
        }
      }
    mScaner.tokenNeed( ']', QStringLiteral("No closing ]") );
    return array;
    }

  else if( mScaner.matchToken( '(' ) ) {
    // ( expr )
    SdScriptValue *val = parseExpression();
    mScaner.tokenNeed( ')', QStringLiteral("No closing )") );
    return val;
    }

  return failValue();
  }




SdScriptValue *SdM3dParser::parseFunction(const QString &functionName)
  {
  if( mFunctions.contains(functionName) ) {
    SdM3dFunction *function = mFunctions.value(functionName) ();
    for( int i = 0; i < function->paramCount(); i++ ) {
      SdScriptValue *param = parseExpression();
      if( param->type() != function->paramType(i) ) {
        mScaner.error( QStringLiteral("Invalid function param %1").arg(i) );
        delete param;
        delete function;
        return failValue();
        }
      function->paramSet( i, param );

      if( i + 1 < function->paramCount() ) {
        if( !mScaner.tokenNeed( ',', QStringLiteral("Need comma") ) ) {
          delete function;
          return failValue();
          }
        }
      else {
        if( !mScaner.tokenNeed( ')', QStringLiteral("Need closing )") ) ) {
          delete function;
          return failValue();
          }
        }
      }
    return function;
    }

  mScaner.error( QStringLiteral("No function '%1'").arg(functionName) );
  return failValue();
  }




SdScriptValue *SdM3dParser::failValue() const
  {
  return new SdScriptValue();
  }


