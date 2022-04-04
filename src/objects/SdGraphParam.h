/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Any param in sheet
*/
#ifndef SDGRAPHPARAM_H
#define SDGRAPHPARAM_H

#include "SdGraphText.h"
#include "SdPropText.h"
#include "SdPoint.h"

#define SD_TYPE_PARAM "Param"

class SdGraphParam : public SdGraphText
  {
  public:
    SdGraphParam();
    SdGraphParam( SdPoint org, const QString &str, SdRect r, const SdPropText &p );

    QString paramName() const;

    QString paramValueGet() const;

    void    paramValueSet(const QString &value );

    QString paramText() const;

    void    paramParse(const QString &param);

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_PARAM); }
    virtual quint64 getClass() const override { return dctParam; }

    // SdGraph interface
  public:
    virtual void    setText(int index, QString sour, SdPropText &prop, QWidget *parent) override;
  };

#endif // SDGRAPHPARAM_H
