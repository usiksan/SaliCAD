#ifndef SDGRAPHSCRIPTREF_H
#define SDGRAPHSCRIPTREF_H

#include "SdJsonIO.h"
#include "SdGraphParam.h"


class SdGraphScriptRef
  {
    mutable SdContainer  *mRefOwner;
    mutable SdGraphParam *mRef;
            QString       mName;
            QString       mParam;
            QString       mValue;
            SdRect        mOverName;
            SdRect        mOverValue;
  public:
    SdGraphScriptRef();
    SdGraphScriptRef( const QString &name );

    QString name() const { return mName; }

    SdRect  overName() const { return mOverName; }

    SdRect  overValue() const { return mOverValue; }

    QString valueGet() const;

    void    valueSet( const QString &v );

    void    assign( SdContainer *refOwner, SdGraphParam *ref, QString param );

    void    jsonWrite( SdJsonWriter &js ) const;

    void    jsonRead( SdJsonReader &js );

  private:
    bool    checkRef() const;
  };

#endif // SDGRAPHSCRIPTREF_H
