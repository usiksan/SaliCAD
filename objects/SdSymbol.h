/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDSYMBOL_H
#define SDSYMBOL_H

#include "SdProjectItem.h"

#define SD_TYPE_SYMBOL "Symbol"

class SdSymbol : public SdProjectItem
  {
  public:
    SdSymbol();

    virtual QString   getType() const override;
    virtual quint64   getClass() const override;
    virtual QString   getIconName() const override;
    virtual void      cloneFrom(SdObject *src) override;
  };

#endif // SDSYMBOL_H
