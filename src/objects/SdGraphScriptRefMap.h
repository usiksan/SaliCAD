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
    QList<SdGraphScriptRef> mRefList;  //!< List of all referenced variables. Sort by date appendion
    QMap<QString,int>       mRefMap;   //!< Map to speed up access to variables. It associates variable key and index in variable list mRefList
    SdRect                  mOverRect; //!< Full over rect of visual representation of all variables in map
  public:
    SdGraphScriptRefMap();

    int    count() const { return mRefList.count(); }

    void   setText(int index, QString sour);
    SdRect getOverRect() const { return mOverRect; }
    int    tryLink( SdPoint p, SdRect &overRect ) const;
    void   link(int index, SdContainer *owner, SdGraphParam *ref, const QString paramName );
    void   drawExcept( SdPoint p, const SdPropText &prop, SdContext *dc, int exceptIndex );

    //!
    //! \brief drawLinks Draw lines connected each variable with its referenced param
    //! \param dc        Draw context
    //!
    void   drawLinks( SdPoint org, SdContext *dc );
    int    behindText( SdPoint p, SdPoint &org, QString &dest );

    void   cloneLinks( QPoint org, const SdGraphScriptRefMap &refMap );
    void   updateLinks( QPoint org, SdContainer *sheet );

    //!
    //! \brief fillLinks Fills mRefLink fields in SdGraphScriptRef
    //!
    void   fillLinks( SdPoint org );

    //!
    //! \brief clearCalculations Reset mCalculated field in SdGraphScriptRef
    //!                          so that it is correctly set during the calculation
    //!
    void   clearCalculated();

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
