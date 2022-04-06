#ifndef SDSCRIPTPARAMREF_H
#define SDSCRIPTPARAMREF_H


#include "objects/SvJsonIO.h"
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

    void    jsonWrite( SvJsonWriter &js ) const;

    void    jsonRead( SvJsonReaderExt<SdObjectMap> &js );

  private:
    bool    checkRef();
  };

using SdScriptParamRefMap = QMap<QString,SdScriptParamRef>;

#endif // SDSCRIPTPARAMREF_H
