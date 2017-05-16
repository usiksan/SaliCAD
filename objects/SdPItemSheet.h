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

class SdPItemSheet : public SdProjectItem
  {
    int mSheetIndex; //Sheet index
  public:
    SdPItemSheet();

    //get net by its name
    SdContainerSheetNet* getNet( const QString name );
    //Creates net with desired name or return existing net
    SdContainerSheetNet* createNet( const QString name );
    //Get net name in point
    bool                 getNetFromPoint( SdPoint p, QString &dest );
    //Get visual sheet index
    int                  getSheetIndex() const { return mSheetIndex; }
    //Set visual sheet index
    void                 setSheetIndex( int index ) { mSheetIndex = index; }
    //TODO Определить к какой плате относится точка
    //int           GetPlateIndex( DPoint point );                //Получить индекс платы исходя из точки на схеме
    //Get plate from point. Plate may be default pcb or pcb of area
    SdPItemPlate *getPlate( SdPoint p );

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;

    // SdProjectItem interface
  public:
    virtual QString getIconName() const override;
  };

#endif // SDPITEMSHEET_H
