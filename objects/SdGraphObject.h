/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDGRAPHOBJECT_H
#define SDGRAPHOBJECT_H

#include "SdObject.h"
#include "SdPoint.h"
#include "SdRect.h"
#include "SdProp.h"

class SdSelector;
class SdContext;
class SdSnapInfo;

class SdGraphObject : public SdObject
  {
  protected:
    SdSelector *mSelector;
  public:
    SdGraphObject();

    //Select object
    SdSelector*     getSelector() { return mSelector; }
    void            doSelect( SdSelector *selector );

    //Изменение перемещением
     virtual void   move( SdPoint offset );
     virtual void   rotate( SdPoint center, SdAngle angle );
     virtual void   mirror( SdPoint a, SdPoint b );
     //virtual void   scale( double factor ) {}

     //Изменение свойствами
     virtual void   setProp( SdProp &prop );
     virtual void   getProp( SdProp &prop );

     //Изменение обновлением текста
     virtual void   setText(int index, QString sour, SdTextProp &prop );

     //Варианты выделения
     virtual void   selectByPoint( const SdPoint p, SdSelector *selector );
     virtual void   selectByRect( const SdRect &r, SdSelector *selector );
     virtual void   select( SdSelector *selector );
     //Обработка слоев
     virtual void   correctLayers() {}   //Коррекция слоев графических объектов
     virtual bool   canDelLayer( int layer );
     //Обработка шрифтов
     virtual void   correctFonts() {}     //Коррекция шрифтов графических объектов
     virtual bool   canDelFont( int layer );
     //Проверить доступность объекта
     virtual bool   isAble() { return false; }
     //Получить охватывающий прямоугольник
     virtual SdRect getOverRect() const = 0;
     //Отображения объекта
     virtual void   draw( SdContext *dc );              //Рисование объекта на экране
     //Получить состояние объета под курсором
     virtual int    behindCursor( SdPoint p );
     virtual int    behindText( SdPoint p, QString &dest, SdTextProp &prop );
     virtual bool   getInfo( SdPoint p, QString &info, bool extInfo );
     //Определение разумной точки
     virtual bool   snapPoint( SdSnapInfo &snap );

  };

#endif // SDGRAPHOBJECT_H
