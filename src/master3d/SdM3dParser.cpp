
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
#include "SdM3dVariableText.h"
#include "SdM3dVariablePin.h"

//Predefined variable to insert faces into part
#include "SdM3dPartModel.h"

#include "SdM3dValue.h"
#include "SdM3dFloat.h"
#include "SdM3dString.h"

#include "SdM3dArrayVertex.h"
#include "SdM3dArraySegment.h"
#include "SdM3dArrayFace.h"

#include "SdM3dBinaryFloatMult.h"
#include "SdM3dBinaryFloatDiv.h"
#include "SdM3dBinaryFloatAdd.h"
#include "SdM3dBinaryFloatSub.h"
#include "SdM3dUnaryFloatMinus.h"
#include "SdM3dReference.h"

#include "SdM3dBinaryBoolAnd.h"
#include "SdM3dBinaryBoolOr.h"
#include "SdM3dBinaryBoolFloatLess.h"
#include "SdM3dUnaryBoolNot.h"

//Functions
#include "SdM3dFunInputFloat.h"
#include "SdM3dFunInputColor.h"
#include "SdM3dFunFaceBuild.h"
#include "SdM3dFunColorBuild.h"
#include "SdM3dFunColorFromString.h"
#include "SdM3dFunVertexBuild.h"
#include "SdM3dFunVertexOffset.h"
#include "SdM3dFunVertexTranslate.h"
#include "SdM3dFunVertexCenterOfRegion.h"
#include "SdM3dFunRegionRect.h"
#include "SdM3dFunRegionTranslate.h"
#include "SdM3dFunFaceTranslate.h"
#include "SdM3dFunModelWall.h"
#include "SdM3dFunModelWallEven.h"
#include "SdM3dFunModelExtrude.h"

SdM3dParser::SdM3dParser(QTableWidget *tableWidget)
  {
  //Fill functions
  addFunction( QStringLiteral("inputFloat"), [tableWidget] () -> SdM3dFunction* { return new SdM3dFunInputFloat( tableWidget ); } );
  addFunction( QStringLiteral("inputColor"), [tableWidget] () -> SdM3dFunction* { return new SdM3dFunInputColor( tableWidget ); } );

  addFunction( QStringLiteral("color"), [] () -> SdM3dFunction* { return new SdM3dFunColorBuild(); } );
  addFunction( QStringLiteral("colorFromString"), [] () -> SdM3dFunction* { return new SdM3dFunColorFromString(); } );

  addFunction( QStringLiteral("vertex"), [] () -> SdM3dFunction* { return new SdM3dFunVertexBuild(); } );
  addFunction( QStringLiteral("vertexOffset"), [] () -> SdM3dFunction* { return new SdM3dFunVertexOffset(); } );
  addFunction( QStringLiteral("vertexTranslate"), [] () -> SdM3dFunction* { return new SdM3dFunVertexTranslate(); } );
  addFunction( QStringLiteral("vertexCenterOfRegion"), [] () -> SdM3dFunction* { return new SdM3dFunVertexCenterOfRegion(); } );

  addFunction( QStringLiteral("regionRect"), [] () -> SdM3dFunction* { return new SdM3dFunRegionRect(); } );
  addFunction( QStringLiteral("regionTranslate"), [] () -> SdM3dFunction* { return new SdM3dFunRegionTranslate(); } );

  addFunction( QStringLiteral("face"), [] () -> SdM3dFunction* { return new SdM3dFunFaceBuild(); } );
  addFunction( QStringLiteral("faceTranslate"), [] () -> SdM3dFunction* { return new SdM3dFunFaceTranslate(); } );

  addFunction( QStringLiteral("modelWall"), [] () -> SdM3dFunction* { return new SdM3dFunModelWall(); } );
  addFunction( QStringLiteral("modelWallEven"), [] () -> SdM3dFunction* { return new SdM3dFunModelWallEven(); } );
  addFunction( QStringLiteral("modelExtrude"), [] () -> SdM3dFunction* { return new SdM3dFunModelExtrude(); } );
  }




//!
//! \brief parse Execute parsing of programm source text and generates tree of programm
//! \param src   Programm source text
//! \param part  Part to which will be placed generated faces
//! \return      Programm tree
//!
SdM3dProgramm *SdM3dParser::parse(const QString src, SdPItemPart *part )
  {
  //Insert predefined variables
  mVariables.insert( QStringLiteral("partModel"), new SdM3dPartModel(part) );

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

  SdM3dValue *val = parseExpression();

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
    case SDM3D_TYPE_BOOL    : var = new SdM3dVariableBool(); break;
    case SDM3D_TYPE_FLOAT   : var = new SdM3dVariableFloat(); break;
    case SDM3D_TYPE_COLOR   : var = new SdM3dVariableColor(); break;
    case SDM3D_TYPE_STRING  : var = new SdM3dVariableString(); break;
    case SDM3D_TYPE_VERTEX  : var = new SdM3dVariableVertex(); break;
    case SDM3D_TYPE_MATRIX  : var = new SdM3dVariableMatrix(); break;
    case SDM3D_TYPE_SEGMENT : var = new SdM3dVariableSegment(); break;
    case SDM3D_TYPE_PATH    : var = new SdM3dVariablePath(); break;
    case SDM3D_TYPE_REGION  : var = new SdM3dVariableRegion(); break;
    case SDM3D_TYPE_FACE    : var = new SdM3dVariableFace(); break;
    case SDM3D_TYPE_MODEL   : var = new SdM3dVariableModel(); break;
    case SDM3D_TYPE_GRAPH   : var = new SdM3dVariableGraph(); break;
    case SDM3D_TYPE_TEXT    : var = new SdM3dVariableText(); break;
    case SDM3D_TYPE_PIN     : var = new SdM3dVariablePin(); break;
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

  SdM3dValue *condition = parseExpression();
  if( condition->type() != SDM3D_TYPE_BOOL ) {
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

  SdM3dValue *condition = parseExpression();
  if( condition->type() != SDM3D_TYPE_BOOL ) {
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




SdM3dValue *SdM3dParser::parseExpression()
  {
  return parseAnd();
  }




SdM3dValue *SdM3dParser::parseAnd()
  {
  SdM3dValue *val = parseOr();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('&') ) {
      SdM3dValue *val2 = parseOr();
      if( val->type() != SDM3D_TYPE_BOOL || val2->type() != SDM3D_TYPE_BOOL ) {
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




SdM3dValue *SdM3dParser::parseOr()
  {
  SdM3dValue *val = parseNot();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('|') ) {
      SdM3dValue *val2 = parseNot();
      if( val->type() != SDM3D_TYPE_BOOL || val2->type() != SDM3D_TYPE_BOOL ) {
        mScaner.error( QStringLiteral("Invalid types of OR operation. Both must be bool.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdM3dBinaryBoolOr( val, val2 );
      }
    else break;
    }
  return val;
  }




SdM3dValue *SdM3dParser::parseNot()
  {
  if( mScaner.matchToken( '!' ) ) {
    SdM3dValue *val = parseNot();
    if( val->type() != SDM3D_TYPE_BOOL ) {
      mScaner.error( QStringLiteral("Invalid type of unary not operation. Must be bool.") );
      delete val;
      return failValue();
      }
    return new SdM3dUnaryBoolNot( val );
    }
  return parsePlusMinus();
  }



SdM3dValue *SdM3dParser::parsePlusMinus()
  {
  SdM3dValue *val = parseMultDiv();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('+') ) {
      SdM3dValue *val2 = parseMultDiv();
      if( val->type() != SDM3D_TYPE_FLOAT || val2->type() != SDM3D_TYPE_FLOAT ) {
        mScaner.error( QStringLiteral("Invalid types of add operation. Both must be float.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdM3dBinaryFloatAdd( val, val2 );
      }
    else if( mScaner.matchToken('-') ) {
      SdM3dValue *val2 = parseMultDiv();
      if( val->type() != SDM3D_TYPE_FLOAT || val2->type() != SDM3D_TYPE_FLOAT ) {
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





SdM3dValue *SdM3dParser::parseMultDiv()
  {
  SdM3dValue *val = parseMinus();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('*') ) {
      SdM3dValue *val2 = parseMinus();
      if( val->type() != SDM3D_TYPE_FLOAT || val2->type() != SDM3D_TYPE_FLOAT ) {
        mScaner.error( QStringLiteral("Invalid types of multiply operation. Both must be float.") );
        delete val;
        delete val2;
        return failValue();
        }
      val = new SdM3dBinaryFloatMult( val, val2 );
      }
    else if( mScaner.matchToken('/') ) {
      SdM3dValue *val2 = parseMinus();
      if( val->type() != SDM3D_TYPE_FLOAT || val2->type() != SDM3D_TYPE_FLOAT ) {
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





SdM3dValue *SdM3dParser::parseMinus()
  {
  if( mScaner.matchToken( '-' ) ) {
    SdM3dValue *val = parseMinus();
    if( val->type() != SDM3D_TYPE_FLOAT ) {
      mScaner.error( QStringLiteral("Invalid type of unary minus operation. Must be float.") );
      delete val;
      return failValue();
      }
    return new SdM3dUnaryFloatMinus( val );
    }
  return parseVar();
  }







SdM3dValue *SdM3dParser::parseVar()
  {
  if( mScaner.token() == 'n' ) {
    //Variable or function
    QString name = mScaner.tokenValue();
    mScaner.tokenNext();
    if( mScaner.matchToken('(') )
      return parseFunction( name );

    if( mVariables.contains(name) )
      return new SdM3dReference( mVariables.value(name) );

    mScaner.error( QStringLiteral("Undefined variable '%1'").arg(name) );
    return failValue();
    }

  else if( mScaner.token() == 'd' ) {
    //float
    SdM3dValue *val = new SdM3dFloat( mScaner.tokenValueFloat() );
    mScaner.tokenNext();
    return val;
    }

  else if( mScaner.token() == 's' ) {
    //String
    SdM3dValue *val = new SdM3dString( mScaner.tokenValue() );
    mScaner.tokenNext();
    return val;
    }

  else if( mScaner.matchToken( '[' ) ) {
    //list object
    // [ 1, 2, 3, 4 ]
    SdM3dValue *val = parseExpression();
    SdM3dArray *array;
    if( val->type() == SDM3D_TYPE_VERTEX )
      array = new SdM3dArrayVertex();
    else if( val->type() == SDM3D_TYPE_SEGMENT )
      array = new SdM3dArraySegment();
    else if( val->type() == SDM3D_TYPE_FACE )
      array = new SdM3dArrayFace();
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
    SdM3dValue *val = parseExpression();
    mScaner.tokenNeed( ')', QStringLiteral("No closing )") );
    return val;
    }

  return failValue();
  }




SdM3dValue *SdM3dParser::parseFunction(const QString &functionName)
  {
  if( mFunctions.contains(functionName) ) {
    SdM3dFunction *function = mFunctions.value(functionName) ();
    for( int i = 0; i < function->paramCount(); i++ ) {
      SdM3dValue *param = parseExpression();
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




SdM3dValue *SdM3dParser::failValue() const
  {
  return new SdM3dValue();
  }


