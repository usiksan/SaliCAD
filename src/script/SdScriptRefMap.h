#ifndef SDSCRIPTREFMAP_H
#define SDSCRIPTREFMAP_H

#include <QString>

class SdScriptRefMap
  {
  public:
    SdScriptRefMap() {}
    virtual ~SdScriptRefMap() {}

    virtual void    varInit( const QString &key ) = 0;

    virtual QString varGet( const QString &key ) const = 0;

    virtual void    varSet( const QString &key, const QString &val ) = 0;
  };

#endif // SDSCRIPTREFMAP_H
