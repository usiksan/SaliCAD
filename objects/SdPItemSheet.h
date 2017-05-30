/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical schematic sheet presentation
*/
#ifndef SDPITEMSHEET_H
#define SDPITEMSHEET_H

#include "SdProjectItem.h"

#define SD_TYPE_SHEET "Sheet"

class SdPItemPlate;
class SdContainerSheetNet;
class SdGraphWiringWire;

class SdPItemSheet : public SdProjectItem
  {
    int mSheetIndex; //Sheet index
  public:
    SdPItemSheet();

    //get net by its name
    SdContainerSheetNet  *netGet( const QString name );

    //Creates net with desired name or return existing net
    SdContainerSheetNet  *netCreate( const QString name, SdUndo *undo );

    //Rename net. Both simple rename and union two nets
    void                  netRename( const QString oldName, const QString newName, SdUndo *undo );

    //Information about wire segment moving to make connection to pin
    void                  netWirePlace( SdPoint a, SdPoint b, const QString name, SdUndo *undo );

    //Information about wire segment delete to remove connection from pin
    void                  netWireDelete( SdPoint a, SdPoint b, const QString name, SdUndo *undo );

    //Insert wire
    void                  insertWire( const QString name, SdGraphWiringWire *wire, SdUndo *undo );

    //Get net name in point
    bool                  getNetFromPoint( SdPoint p, QString &dest );

    //Get visual sheet index
    int                   getSheetIndex() const { return mSheetIndex; }

    //Set visual sheet index
    void                  setSheetIndex( int index ) { mSheetIndex = index; }

    //Get plate from point. Plate may be default pcb or pcb of area
    SdPItemPlate         *getPlate( SdPoint p );

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

#endif // SDPITEMSHEET_H
