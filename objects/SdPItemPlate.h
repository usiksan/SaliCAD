/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical PCB layout with component placement
*/
#ifndef SDPITEMPLATE_H
#define SDPITEMPLATE_H

#include "SdProjectItem.h"

class SdGraphPartImp;
class SdGraphSymImp;
class SdPItemPart;
class SdPItemSymbol;
class SdContainerPlateNet;

#define SD_TYPE_PLATE "Plate"

class SdPItemPlate : public SdProjectItem
  {
  public:
    SdPItemPlate();

    //get net by its name
    SdContainerPlateNet  *netGet( const QString name );

    //Creates net with desired name or return existing net
    SdContainerPlateNet  *netCreate( const QString name, SdUndo *undo );


    SdGraphPartImp       *allocPartImp(int *section, SdPItemPart *part, SdPItemSymbol *comp, SdPItemSymbol *sym , SdUndo *undo);
//    DPrtImpPic*  FindCompPart( DPrtPic *part, CPChar name, int numSection );
//    DPrtImpPic*  FindCompById( CPChar ident );

    // SdObject interface
  public:
    virtual QString       getType() const override;
    virtual quint64       getClass() const override;
    virtual void          cloneFrom(const SdObject *src) override;

    // SdProjectItem interface
  public:
    virtual QString       getIconName() const override;
    virtual SdGraphIdent *createIdent() override;
  };

#endif // SDPITEMPLATE_H
