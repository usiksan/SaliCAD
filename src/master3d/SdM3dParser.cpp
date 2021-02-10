#include "SdM3dParser.h"
#include "SdM3dOperatorBlock.h"
#include "SdM3dOperatorAssign.h"
#include "SdM3dVariableFloat.h"
#include "SdM3dValue.h"

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

  }


SdM3dValue *SdM3dParser::parseVar()
  {
  if( mScaner.token() == 'n' ) {
    //Variable or function
    }
  else if( mScaner.token() == 'd' ) {
    //float
    }
  }


