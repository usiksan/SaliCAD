#include "SdExpression.h"

#include <math.h>
#include <QSet>


class SdExpressBase {
  public:
    virtual ~SdExpressBase() {}

    virtual double value( SdExpression *expression ) const = 0;
  };

class SdExpressParam : public SdExpressBase {
    QString mParamName;
  public:
    SdExpressParam( const QString paramName ) : mParamName(paramName) {}

    virtual double value( SdExpression *expression ) const override { return expression->value(mParamName); }
  };

class SdExpressConst : public SdExpressBase {
    double mValue;
  public:
    SdExpressConst( double val ) : mValue(val) {}

    virtual double value( SdExpression* ) const override { return mValue; }
  };

class SdExpressUnari : public SdExpressBase {
  protected:
    SdExpressBase *mValue;
  public:
    SdExpressUnari(SdExpressBase *val) : mValue(val) {}
    virtual ~SdExpressUnari() { if( mValue != nullptr ) delete mValue; }
  };

class SdExpressUnariPlus : public SdExpressUnari {
  public:
    SdExpressUnariPlus(SdExpressBase *val) : SdExpressUnari(val) {}

    virtual double value( SdExpression *expression ) const override { return mValue->value(expression); }
  };

class SdExpressUnariMinus : public SdExpressUnari {
  public:
    SdExpressUnariMinus(SdExpressBase *val) : SdExpressUnari(val) {}

    virtual double value( SdExpression *expression ) const override { return -mValue->value(expression); }
  };

class SdExpressUnariSquare : public SdExpressUnari {
  public:
    SdExpressUnariSquare(SdExpressBase *val) : SdExpressUnari(val) {}

    virtual double value( SdExpression *expression ) const override { return std::sqrt( mValue->value(expression) ); }
  };



class SdExpressBinari : public SdExpressBase {
  protected:
    SdExpressBase *mValue1;
    SdExpressBase *mValue2;
  public:
    SdExpressBinari(SdExpressBase *val1, SdExpressBase *val2) : mValue1(val1), mValue2(val2) {}
    virtual ~SdExpressBinari() { if(mValue1) delete mValue1; if( mValue2) delete mValue2; }

    void setValue1( SdExpressBase *val ) { mValue1 = val; }
    void setValue2( SdExpressBase *val ) { mValue2 = val; }
  };

class SdExpressBinariPlus : public SdExpressBinari {
  public:
    SdExpressBinariPlus(SdExpressBase *val1, SdExpressBase *val2) : SdExpressBinari(val1,val2) {}

    virtual double value( SdExpression *expression ) const override { return mValue1->value(expression) + mValue2->value(expression); }
  };

class SdExpressBinariMinus : public SdExpressBinari {
  public:
    SdExpressBinariMinus(SdExpressBase *val1, SdExpressBase *val2) : SdExpressBinari(val1,val2) {}

    virtual double value( SdExpression *expression ) const override { return mValue1->value(expression) - mValue2->value(expression); }
  };

class SdExpressBinariMul : public SdExpressBinari {
  public:
    SdExpressBinariMul(SdExpressBase *val1, SdExpressBase *val2) : SdExpressBinari(val1,val2) {}

    virtual double value( SdExpression *expression ) const override { return mValue1->value(expression) * mValue2->value(expression); }
  };

class SdExpressBinariDiv : public SdExpressBinari {
  public:
    SdExpressBinariDiv(SdExpressBase *val1, SdExpressBase *val2) : SdExpressBinari(val1,val2) {}

    virtual double value( SdExpression *expression ) const override { double d = mValue2->value(expression); if( d == 0 ) d = 0.00001;  return mValue1->value(expression) / d ; }
  };

class SdExpressBinariPow : public SdExpressBinari {
  public:
    SdExpressBinariPow(SdExpressBase *val1, SdExpressBase *val2) : SdExpressBinari(val1,val2) {}

    virtual double value( SdExpression *expression ) const override { return std::pow( mValue1->value(expression), mValue2->value(expression) ); }
  };


class SdExpressionParser {
    SdExpressionPairMap &mMap;
    QSet<QString>        mParamSet;
    QString              mSource;
    QString              mResult;
    QStringList          mParamList;
    int                  mPtr;
    char                 mToken;
    QString              mTokenContents;
    QString              mError;
    int                  mErrorPtr;
  public:
    SdExpressionParser( SdExpressionPairMap &map, const QString src ) : mMap(map), mSource(src), mPtr(0), mErrorPtr(0) {}

    QString        result() const { return mResult; }
    QStringList    paramList() const { return mParamList; }
    QString        error() const { return mError; }
    int            errorPtr() const { return mErrorPtr; }
    SdExpressBase *parse();
  private:
    void           nextToken();

    SdExpressBase *exprPlusMinus();
    SdExpressBase *exprMulDivPow();
    SdExpressBase *exprBrackets();
  };



SdExpression::SdExpression() :
  mMap(),
  mExpression(nullptr)
  {

  }

QString SdExpression::parse(const QString expressionString)
  {
  if( mExpression ) {
    delete mExpression;
    mExpression = nullptr;
    }
  SdExpressionParser parser( mMap, expressionString );
  mExpression = parser.parse();
  mResult = parser.result();
  mParamList = parser.paramList();

  if( parser.error().isEmpty() )
    return parser.error();

  if( mExpression ) {
    delete mExpression;
    mExpression = nullptr;
    }
  return parser.error() + QStringLiteral(" at %1").arg(parser.errorPtr());
  }




void SdExpression::calculate()
  {
  if( mExpression )
    mMap[mResult] = mExpression->value( this );
  }










SdExpressBase *SdExpressionParser::parse()
  {
  nextToken();
  if( mToken == ':' ) {
    nextToken();
    if( mToken == 'p' ) {
      if( !mMap.contains(mTokenContents) )
        mMap.insert( mTokenContents, 0 );
      mResult = mTokenContents;

      nextToken();
      if( mToken == '=' ) {
        nextToken();
        SdExpressBase *exp = exprPlusMinus();
        if( mToken != 0 ) {
          mError = QStringLiteral("Undefined token %1").arg(mToken);
          if( exp ) delete exp;
          return nullptr;
          }
        return exp;
        }
      mError = QStringLiteral("Need =");
      }
    else mError = QStringLiteral("Need name for expression result");
    }
  else mError = QStringLiteral("No expression");
  return nullptr;
  }






void SdExpressionParser::nextToken()
  {
  mToken = 0;
  if( !mError.isEmpty() || mPtr >= mSource.count() )
    return;

  //Blank
  while( mSource.at(mPtr).isSpace() ) {
    mPtr++;
    if( mPtr >= mSource.count() ) return;
    }

  mErrorPtr = mPtr;
  if( mSource.at(mPtr).isDigit() ) {
    mToken = 'd';
    mTokenContents.clear();
    //Digits
    while( mPtr < mSource.count() && mSource.at(mPtr).isDigit() )
      mTokenContents.append( mSource.at(mPtr++) );

    if( mPtr >= mSource.count() ) return;

    //Point
    if( mSource.at(mPtr) == QChar('.') || mSource.at(mPtr) == QChar(',') ) {
      mPtr++;
      mTokenContents.append( QChar('.') );
      //Digits
      while( mPtr < mSource.count() && mSource.at(mPtr).isDigit() )
        mTokenContents.append( mSource.at(mPtr++) );

      if( mPtr >= mSource.count() ) return;

      if( mSource.at(mPtr) == QChar('e') || mSource.at(mPtr) == QChar('E') ) {
        mPtr++;
        mTokenContents.append( QChar('e') );

        if( mPtr >= mSource.count() ) return;

        if( mSource.at(mPtr) == QChar('+') || mSource.at(mPtr) == QChar('-') )
          mTokenContents.append( mSource.at(mPtr++) );

        //Digits
        while( mPtr < mSource.count() && mSource.at(mPtr).isDigit() )
          mTokenContents.append( mSource.at(mPtr++) );
        }
      }
    }

  else if( mSource.at(mPtr).isLetter() ) {
    mToken = 'p';
    mTokenContents.clear();
    //Digits
    while( mPtr < mSource.count() && mSource.at(mPtr).isLetterOrNumber() )
      mTokenContents.append( mSource.at(mPtr++) );
    }

  else {
    mToken = mSource.at(mPtr++).toLatin1();
    }
  }




SdExpressBase *SdExpressionParser::exprPlusMinus()
  {
  SdExpressBase *val1 = exprMulDivPow();
  while(true) {
    if( mToken == '+' ) {
      nextToken();
      val1 = new SdExpressBinariPlus( val1, exprMulDivPow() );
      }
    else if( mToken == '-' ) {
      nextToken();
      val1 = new SdExpressBinariMinus( val1, exprMulDivPow() );
      }
    else return val1;
    }
  }




SdExpressBase *SdExpressionParser::exprMulDivPow()
  {
  SdExpressBase *val1 = exprBrackets();
  while(true) {
    if( mToken == '*' ) {
      nextToken();
      val1 = new SdExpressBinariMul( val1, exprBrackets() );
      }
    else if( mToken == '/' ) {
      nextToken();
      val1 = new SdExpressBinariDiv( val1, exprBrackets() );
      }
    else if( mToken == '^' ) {
      nextToken();
      val1 = new SdExpressBinariPow( val1, exprBrackets() );
      }
    else return val1;
    }
  }





SdExpressBase *SdExpressionParser::exprBrackets()
  {
  SdExpressBase *expr = nullptr;
  if( mToken == '(' ) {
    nextToken();
    expr = exprPlusMinus();
    if( mToken != ')' )
      mError = QStringLiteral("Need )");
    else nextToken();
    }
  else if( mToken == 'p' ) {
    if( !mMap.contains(mTokenContents) )
      mMap.insert(mTokenContents,0);
    if( !mParamSet.contains(mTokenContents) ) {
      mParamSet.insert( mTokenContents );
      mParamList.append( mTokenContents );
      }
    expr = new SdExpressParam(mTokenContents);
    nextToken();
    }
  else if( mToken == 'd' ) {
    expr = new SdExpressConst( mTokenContents.toDouble() );
    nextToken();
    }
  else mError = QStringLiteral("Undefined token %1").arg(mToken);
  return expr;
  }
