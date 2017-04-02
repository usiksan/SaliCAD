/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDMODECOMMON_H
#define SDMODECOMMON_H

#include "SdMode.h"

class SdModeCommon : public SdMode
  {
  protected:
    SdPoint mPrev; //Предыдущая точка (при перемещении)
  public:
    SdModeCommon( SdWEditorGraph *editor, SdProjectItem *obj ) : SdMode( editor, obj ) {}

    //Определяются здесь
    virtual int  getPropBarId() const = 0;          //Получить идентификатор панели свойств
    virtual void setPropertais( SdProp &prop ) = 0; //Установить свойства
    virtual void movePointLow( SdPoint ) {}         //Перемещение точки

    //Наследуются из SdMode
    virtual void activate();                         //Вызывается первой, после того как режим делается текущим
    virtual void propChanged( SdContext& );          //Извещение об изменении свойств
    virtual void movePoint( SdPoint, SdContext& );   //Ввод точки (левая кнопка)

  };

#endif // SDMODECOMMON_H
