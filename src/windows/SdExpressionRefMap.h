#ifndef SDEXPRESSIONREFMAP_H
#define SDEXPRESSIONREFMAP_H

#include "script/SdScriptRefMap.h"
#include <QTableWidget>
#include <QMap>

class SdExpressionRefMap : public SdScriptRefMap
  {
    QTableWidget      *mTable;
    QList<QString>     mRefList;
    QMap<QString,int>  mRefMap;
  public:
    SdExpressionRefMap( QTableWidget *table );

    void parseEnd();
    // SdScriptRefMap interface
  public:
    virtual void varInit(const QString &key) override;
    virtual QString varGet(const QString &key) const override;
    virtual void varSet(const QString &key, const QString &val) override;
  };

#endif // SDEXPRESSIONREFMAP_H
