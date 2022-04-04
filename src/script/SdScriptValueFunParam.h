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

  Embedded function.

  This function appends one field to program param table as component parameter or sheet parameter
*/
#ifndef SDSCRIPTVALUEFUNPARAM_H
#define SDSCRIPTVALUEFUNPARAM_H

#include "SdScriptValueFunction.h"

#include <QTableWidget>


class SdScriptValueFunParam : public SdScriptValueFunction
  {
  protected:
    QTableWidget *mTableWidget; //!< Param table widget
    int           mRow;         //!< Row number for this param
  public:
    SdScriptValueFunParam( QTableWidget *tableWidget );

    QString dimensions() const { return mParamList[1]->toString();}

    QString componentId() const { return mParamList[2]->toString(); }

    QString paramId() const { return mParamList[3]->toString(); }

    QString row() const { return mParamList[4]->toString(); }

    void    titleSet();

    void    valueSet( const QString val );

    // SdScriptValue interface
  public:
    virtual QString toString() const override;
  };

#endif // SDSCRIPTVALUEFUNPARAM_H
