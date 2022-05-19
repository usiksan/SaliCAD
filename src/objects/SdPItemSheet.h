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

#define SD_SHEET_INDEX_PARAM "{sindex}" //Sheet index in sheet parameters table

class SdPItemPlate;
class SdGraphNetWire;

class SdPItemSheet : public SdProjectItem
  {
    int         mSheetIndex; //!< Sheet index
  public:
    SdPItemSheet();

    //get net by its name
    bool                   isNetPresent( const QString name );

    //Rename net. Both simple rename and union two nets
    void                   netRename( const QString oldName, const QString newName, SdUndo *undo );

    //Information about wire segment moving to make connection to pin
    void                   netWirePlace( SdGraphNetWire *wire, SdUndo *undo );

    //Information about wire segment delete to remove connection from pin
    void                   netWireDelete(SdGraphNetWire *wire, SdUndo *undo );

    //Get net name in point
    bool                   getNetFromPoint( SdPoint p, QString &dest );

    //Get visual sheet index
    int                    getSheetIndex() const { return paramGet( QString(SD_SHEET_INDEX_PARAM) ).toInt(); }

    //Set visual sheet index
    void                   setSheetIndex( int index, SdUndo *undo ) { paramSet( QString(SD_SHEET_INDEX_PARAM), QString::number( index ), undo ); }

    //Get plate from point. Plate may be default pcb or pcb of area
    SdPItemPlate          *getPlate( SdPoint p );

    //Accumulate to selector element linked with point and net name
    void                   accumLinked( SdPoint a, SdPoint b, const QString netName, SdSelector *selector, SdUndo *undo );

    // SdObject interface
  public:
    virtual QString        getType() const override;
    virtual SdClass        getClass() const override;
    virtual void           detach(SdUndo *undo) override;

    // SdProjectItem interface
  public:
    virtual QString        getIconName() const override;
    virtual quint64        getAcceptedObjectsMask() const override;

    friend class SdPasCadImport;

  };

#endif // SDPITEMSHEET_H
