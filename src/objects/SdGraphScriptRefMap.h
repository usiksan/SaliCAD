#ifndef SDGRAPHSCRIPTREFMAP_H
#define SDGRAPHSCRIPTREFMAP_H

#include "script/SdScriptRefMap.h"
#include "SdGraphScriptRef.h"


#include <QList>
#include <QMap>

class SdGraphScriptRefMap : public SdScriptRefMap
  {
    QList<SdGraphScriptRef> mRefList;
    QMap<QString,int>       mRefMap;
  public:
    SdGraphScriptRefMap();

    int    count() const { return mRefList.count(); }

    void   setText(int index, QString sour, SdPropText &prop, QWidget *parent);
    SdRect getOverRect() const;
    SdRect draw(SdContext *dc);
    void   drawExcept( SdContext *dc, int exceptIndex );
    int    behindText( SdPoint p, SdPoint &org, QString &dest );

    // SdScriptRefMap interface
  public:
    virtual void parseBegin() override;
    virtual void parseEnd() override;
    virtual void varInit(const QString &key) override;
    virtual QString varGet(const QString &key) const override;
    virtual void varSet(const QString &key, const QString &val) override;
  };

#endif // SDGRAPHSCRIPTREFMAP_H
