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
class SdGraphNetWire;

class SdPItemSheet : public SdProjectItem
  {
    int         mSheetIndex; //Sheet index

    //Not saved
    SdStringMap mNetCopyMap; //When copy schematic fragment named nets copied exactly.
                             //But default named nets must be copied with new default names
                             //This map stores association inserted net name and new net name
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
    int                    getSheetIndex() const { return mSheetIndex; }

    //Set visual sheet index
    void                   setSheetIndex( int index ) { mSheetIndex = index; }

    //Get plate from point. Plate may be default pcb or pcb of area
    SdPItemPlate          *getPlate( SdPoint p );

    //Accumulate to selector element linked with point and net name
    void                   accumLinked( SdPoint a, SdPoint b, const QString netName, SdSelector *selector, SdUndo *undo );

    // SdObject interface
  public:
    virtual QString        getType() const override;
    virtual quint64        getClass() const override;
    virtual void           cloneFrom(const SdObject *src) override;
    virtual void           writeObject(QJsonObject &obj) const override;
    virtual void           readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdProjectItem interface
  public:
    virtual QString        getIconName() const override;
    virtual quint64        getAcceptedObjectsMask() const override;
    virtual SdGraph       *insertCopyObject( const SdGraph *obj, SdPoint offset, SdUndo *undo, SdWEditorGraph *editor, bool next ) override;
    virtual void           insertObjects( SdPoint offset, SdSelector *sour, SdUndo *undo, SdWEditorGraph *editor, SdSelector *dest, bool next ) override;

    friend class SdPasCadImport;
  };

#endif // SDPITEMSHEET_H
