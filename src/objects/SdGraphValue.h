/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph containet object value
*/
#ifndef SDGRAPHVALUE_H
#define SDGRAPHVALUE_H

#include "SdGraphText.h"

#define SD_TYPE_VALUE "Value"

class SdGraphValue : public SdGraphText
  {
  public:
    SdGraphValue();
    SdGraphValue(SdPoint org, SdRect r, SdPropText &p );

    void updateValue( SdPoint org, SdRect r, SdPropText *p );



    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral( SD_TYPE_VALUE ); }
    virtual SdClass getClass() const override { return dctValue; }

    // SdGraph interface
  public:
    virtual void setText(int index, QString sour, SdPropText &prop, QWidget *parent) override;
  };

#endif // SDGRAPHVALUE_H
