#ifndef SDSCRIPTPARAMREF_H
#define SDSCRIPTPARAMREF_H


#include "objects/SdJsonIO.h"
#include "objects/SdGraphParam.h"
#include <QString>
#include <QMap>


class SdScriptParamRef
  {
    SdContainer  *mRefOwner;
    SdGraphParam *mRef;
    QString       mParam;
    QString       mValue;
  public:
    SdScriptParamRef();

    QString valueGet();

    void    valueSet( const QString &v );

    void    assign( SdContainer *refOwner, SdGraphParam *ref, QString param );

    void    json( SdJsonWriter &js ) const;

    void    json( SdJsonReader &js );

  private:
    bool    checkRef();
  };

using SdScriptParamRefMap = QMap<QString,SdScriptParamRef>;

#endif // SDSCRIPTPARAMREF_H
