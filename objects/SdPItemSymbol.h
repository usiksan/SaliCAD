/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical symbol presentation
*/

#ifndef SDPITEMSYMBOL_H
#define SDPITEMSYMBOL_H

#include "SdProjectItem.h"

#define SD_TYPE_SYMBOL "Symbol"

class SdPItemSymbol : public SdProjectItem
  {
  public:
    SdPItemSymbol();

    virtual QString   getType() const override;
    virtual quint64   getClass() const override;
    virtual QString   getIconName() const override;
    virtual void      cloneFrom( const SdObject *src ) override;
  };

#endif // SDPITEMSYMBOL_H
