/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical symbol presentation
  Symbol also may contains symbol sections with pin (name-number) assotiation and component part
*/

#ifndef SDPITEMSYMBOL_H
#define SDPITEMSYMBOL_H

#include "SdProjectItem.h"

class SdPItemSymbol;
class SdPItemPart;
class QWidget;
class SdSection;
class SdPartVariant;


#define SD_TYPE_SYMBOL "Symbol"

class SdPItemSymbol : public SdProjectItem
  {
  public:
    SdPItemSymbol();


    virtual QString   getType() const override;
    virtual quint64   getClass() const override;
    virtual QString   getIconName() const override;
    virtual quint64   getAcceptedObjectsMask() const override;
    //virtual SdGraph  *insertCopyObject( const SdGraph *obj, SdPoint offset, SdUndo *undo, SdWEditorGraph *editor, bool next ) override;
    //virtual void      insertObjects( SdPoint offset, SdSelector *sel, SdUndo *undo, SdWEditorGraph *editor, SdSelector *dest, bool next ) override;

  };

#endif // SDPITEMSYMBOL_H
