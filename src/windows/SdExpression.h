#ifndef SDEXPRESSION_H
#define SDEXPRESSION_H

#include <QString>
#include <QMap>

using SdExpressionPairMap = QMap<QString,double>;

class SdExpressBase;

class SdExpression
  {
    SdExpressionPairMap  mMap;
    QStringList          mParamList;
    QString              mResult;
    SdExpressBase       *mExpression;
    QString              mToken;
    QString              mTokenContents;
  public:
    SdExpression();

    double      value( const QString name ) const { return mMap[name]; }

    void        valueSet( const QString name, double val ) { mMap[name] = val; }

    QStringList paramList() const { return mParamList; }

    QString     result() const { return mResult; }

    QString     parse(const QString expressionString );

    void        calculate();
  };

#endif // SDEXPRESSION_H
