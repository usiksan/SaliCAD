/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph containet object ident
*/
#ifndef SDGRAPHIDENT_H
#define SDGRAPHIDENT_H

#include "SdGraphText.h"

#define SD_TYPE_IDENT "Ident"

class SdGraphIdent : public SdGraphText
  {
  public:
    SdGraphIdent();
    SdGraphIdent( SdPoint org, const QString str, SdRect r, SdPropText &p );

    void updateIdent( SdPoint org, const QString str, SdRect r, SdPropText *p );

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral( SD_TYPE_IDENT ); }
    virtual quint64 getClass() const override { return dctIdent; }

  friend class SdPasCadImport;
  };

#endif // SDGRAPHIDENT_H
