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

class SdPItemSheet : public SdProjectItem
  {
  public:
    SdPItemSheet();

    DSheetNetPic* GetNet( CPChar name );                        //Получить цепь по имени
    DSheetNetPic* CreateNet( CPChar name );                     //Создает цепь с заданным именем или возвращает существующую цепь
    bool          GetNetFromPoint( DPoint p, DName &dest );     //Получить цепь в данной точке
    int           GetIndex() const { return info.index; }       //Получить индекс листа схемы
    void          SetIndex( int index ) { info.index = index; } //Установить индекс листа схемы
    //Определить к какой плате относится точка
    int           GetPlateIndex( DPoint point );                //Получить индекс платы исходя из точки на схеме
    DPlatePic*    GetPlate( DPoint p );

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
