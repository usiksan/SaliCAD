#ifndef SDGRAPHSCRIPTREFMAP_H
#define SDGRAPHSCRIPTREFMAP_H

#include "script/SdScriptRefMap.h"
#include "SdGraphScriptRef.h"
#include "SdPropText.h"
#include "SdContainer.h"


#include <QList>
#include <QMap>


class SdGraphScriptRefMap : public SdScriptRefMap
  {
    QList<SdGraphScriptRef> mRefList;
    QMap<QString,int>       mRefMap;
    SdRect                  mOverRect;
  public:
    SdGraphScriptRefMap();

    int    count() const { return mRefList.count(); }

    void   setText(int index, QString sour);
    SdRect getOverRect() const { return mOverRect; }
    int    tryLink( SdPoint p, SdRect &overRect ) const;
    void   link(int index, SdContainer *owner, SdGraphParam *ref, const QString paramName );
    void   drawExcept( SdPoint p, const SdPropText &prop, SdContext *dc, int exceptIndex );
    int    behindText( SdPoint p, SdPoint &org, QString &dest );

    void   cloneLinks(QPoint org, const SdGraphScriptRefMap &refMap );
    void   updateLinks( QPoint org, SdContainer *sheet );

    void   parseEnd();

    void   jsonWrite( SdJsonWriter &js ) const;
    void   jsonRead( SdJsonReader &js );

    // SdScriptRefMap interface
  public:
    virtual void    varInit(const QString &key) override;
    virtual QString varGet(const QString &key) const override;
    virtual void    varSet(const QString &key, const QString &val) override;

  private:
    QPoint          linkOffset( QPoint org, const QString &key ) const;
    QString         linkParam( const QString &key ) const;
  };

#endif // SDGRAPHSCRIPTREFMAP_H
