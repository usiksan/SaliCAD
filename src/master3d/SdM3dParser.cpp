#include "SdM3dParser.h"
#include "SdM3dOperatorBlock.h"
#include "SdM3dOperatorAssign.h"
#include "SdM3dVariableFloat.h"
#include "SdM3dValue.h"
#include "SdM3dFloat.h"
#include "SdM3dArrayVertex.h"
#include "SdM3dBinaryFloatMult.h"

SdM3dParser::SdM3dParser()
  {

  }

SdM3dOperator *SdM3dParser::parse(const QString src)
  {
  mScaner.sourceSetString( src );
  mScaner.tokenNext();

  SdM3dOperatorBlock *block = new SdM3dOperatorBlock();

  while( !mScaner.isEndOfScanOrError() ) {
    SdM3dOperator *op = parseOperator();
    if( op != nullptr )
      block->append( op );
    }

  return block;
  }

SdM3dOperator *SdM3dParser::parseOperator()
  {
  QString variableName = mScaner.tokenNeedValue('n');
  if( variableName.isEmpty() )
    return nullptr;

  if( !mScaner.tokenNeed('=', QStringLiteral("Need assign =") )  )
    return nullptr;

  SdM3dValue *val = parseExpression();

  if( val == nullptr )
    return nullptr;

  if( mVaribales.contains(variableName) ) {
    SdM3dVariable *var = mVaribales.value(variableName);
    if( var->type() != val->type() ) {
      //Illegal type
      delete val;
      return nullptr;
      }

    return new SdM3dOperatorAssign( var, val );
    }

  //Depending on the type of expression, build a variable
  SdM3dVariable *var = nullptr;
  switch( val->type() ) {
    case SDM3D_TYPE_FLOAT  : var = new SdM3dVariableFloat(); break;
    }
  if( var == nullptr ) {
    //Can't create variable
    delete val;
    return nullptr;
    }
  mVaribales.insert( variableName, var );
  return new SdM3dOperatorAssign( var, val );
  }

SdM3dValue *SdM3dParser::parseExpression()
  {
  SdM3dValue *val = parsePlusMinus();
  }


SdM3dValue *SdM3dParser::parsePlusMinus()
  {
  SdM3dValue *val = parseMultDiv();

  }


SdM3dValue *SdM3dParser::parseMultDiv()
  {
  SdM3dValue *val = parseVar();

  while( !mScaner.isEndOfScanOrError() ) {
    if( mScaner.matchToken('*') ) {
      SdM3dValue *val2 = parseVar();
      if( val->type() != SDM3D_TYPE_FLOAT || val2 == nullptr || val2->type() != SDM3D_TYPE_FLOAT ) {
        mScaner.error( QStringLiteral("Invalid types of multiply operation. Both must be float.") );
        delete val;
        return val2;
        }
      val = new SdM3dBinaryFloatMult( val, val2 );
      }
    else if( mScaner.matchToken('/') ) {
      SdM3dValue *val2 = parseVar();
      if( val->type() != SDM3D_TYPE_FLOAT || val2 == nullptr || val2->type() != SDM3D_TYPE_FLOAT ) {
        mScaner.error( QStringLiteral("Invalid types of multiply operation. Both must be float.") );
        delete val;
        return val2;
        }
      }
    else break;
    }
  return val;
  }


SdM3dValue *SdM3dParser::parseVar()
  {
  if( mScaner.token() == 'n' ) {
    //Variable or function
    }

  else if( mScaner.token() == 'd' ) {
    //float
    SdM3dValue *val = new SdM3dFloat( mScaner.tokenValueFloat() );
    mScaner.tokenNext();
    return val;
    }

  else if( mScaner.matchToken( '[' ) ) {
    //list object
    // [ 1, 2, 3, 4 ]
    SdM3dValue *val = parseExpression();
    SdM3dArray *array = nullptr;
    if( val->type() == SDM3D_TYPE_VERTEX )
      array = new SdM3dArrayVertex();

    if( array == nullptr ) {
      mScaner.error( QStringLiteral("Invalid element type of array. Must be vertex, segment or face") );
      delete val;
      return nullptr;
      }

    array->append( val );
    while( mScaner.matchToken( ',' )  ) {
      val = parseExpression();
      if( val == nullptr ) break;
      array->append( val );
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

  return nullptr;
  }


