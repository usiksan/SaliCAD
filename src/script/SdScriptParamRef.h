#ifndef SDSCRIPTPARAMREF_H
#define SDSCRIPTPARAMREF_H

#include "objects/SvJsonIO.h"
#include "objects/SdObject.h"
#include <QString>

class SdObject;

class SdScriptParamRef
  {
    SdObject *mRef;
    QString   mParam;
    QString   mValue;
  public:
    SdScriptParamRef();

    QString valueGet() const;

    void    valueSet( const QString &v );

    void    assign( SdObject *ref, QString param );

    void    jsonWrite( SvJsonWriter &js ) const;

    void    jsonRead( SvJsonReaderExt<SdObjectMap> &js );
  };

#endif // SDSCRIPTPARAMREF_H
