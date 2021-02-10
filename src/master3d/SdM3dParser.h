#ifndef SDM3DPARSER_H
#define SDM3DPARSER_H

#include "SdM3dValue.h"
#include "SdM3dOperator.h"
#include "SdM3dScaner.h"
#include "SdM3dVariable.h"

#include <QMap>

class SdM3dParser
  {
    SdM3dScaner                  mScaner;
    QMap<QString,SdM3dVariable*> mVaribales;
  public:
    SdM3dParser();

    SdM3dOperator *parse( const QString src );

  private:
    SdM3dOperator *parseOperator();
    SdM3dValue    *parseExpression();
    SdM3dValue    *parseMultDiv();
    SdM3dValue    *parsePlusMinus();
    //SdM3dValue    *parseMult();
    SdM3dValue    *parseVar();
    //parenthesis
  };

#endif // SDM3DPARSER_H
