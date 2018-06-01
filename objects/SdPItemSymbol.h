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

    //Return section count
    int              getSectionCount() const;

    //Return section by index
    SdSection       *getSection( int sectionIndex ) const;

    //Return default part
    SdPartVariant   *getPart() const;



    //Return symbol from section by index
    SdPItemSymbol    *extractSymbolFromFactory( int sectionIndex, bool soft, QWidget *parent ) const;
    QString           getSectionSymbolId( int sectionIndex ) const;

    //Return part descripted part variant
    SdPItemPart      *extractPartFromFactory( bool soft, QWidget *parent ) const;
    QString           getPartId() const;



    virtual QString   getType() const override;
    virtual quint64   getClass() const override;
    virtual QString   getIconName() const override;
    virtual quint64   getAcceptedObjectsMask() const override;
    //virtual SdGraph  *insertCopyObject( SdGraph *obj, SdPoint offset, SdUndo *undo, SdWEditorGraph *editor, bool next ) override;
    //virtual void      insertObjects( SdPoint offset, SdSelector *sel, SdUndo *undo, SdWEditorGraph *editor, SdSelector *dest, bool next ) override;
    virtual void      cloneFrom( const SdObject *src ) override;
    virtual void      insertChild(SdObject *child, SdUndo *undo) override;
  };

#endif // SDPITEMSYMBOL_H
