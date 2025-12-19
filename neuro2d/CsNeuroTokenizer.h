#ifndef CSNEUROTOKENIZER_H
#define CSNEUROTOKENIZER_H

#include <QMap>
#include <QSet>

class CsNeuroTokenizer
  {
    QMap<QString,int> mTokens;
    QMap<QString,int> mNumbers;
    QSet<QString>     mWater;
    QList<double>     mValues;
  public:
    CsNeuroTokenizer();

    int        tokenCount() const { return mTokens.count() + 4; }

    QList<int> tokenize( const QString &src );
  };

#endif // CSNEUROTOKENIZER_H
