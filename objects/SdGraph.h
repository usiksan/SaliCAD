/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base for objects with graphical presentation
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
class QWidget;

class SdGraph : public SdObject
  {
  protected:
    SdSelector *mSelector;
  public:
    SdGraph();

    //Select object
    SdSelector*    getSelector() { return mSelector; }

    //Save object state
    virtual void   saveState( SdUndo *undo );
    virtual void   moveComplete( SdPoint grid, SdUndo *undo );

    //Изменение перемещением
    virtual void   move( SdPoint offset );
    virtual void   rotate( SdPoint center, SdAngle angle );
    virtual void   mirror( SdPoint a, SdPoint b );

    //Изменение свойствами
    virtual void   setProp( SdProp &prop );
    virtual void   getProp( SdProp &prop );

    //Изменение обновлением текста
    virtual void   setText(int index, QString sour, SdPropText &prop, QWidget *parent );

    //Варианты выделения
    virtual void   selectByPoint( const SdPoint p, SdSelector *selector );
    virtual void   selectByRect( const SdRect &r, SdSelector *selector );
    virtual void   select( SdSelector *selector );
    virtual void   prepareMove();
    //Обработка слоев
    virtual bool   canHideLayer( SdLayer *layer );
    //Check if object is visible
    virtual bool   isVisible() { return false; }
    //Get over rect for visible
    virtual SdRect getOverRect() const = 0;
    //Drawing object
    virtual void   draw( SdContext *dc );              //Рисование объекта на экране
    //Get object state behind cursor
    virtual int    behindCursor( SdPoint p );
    virtual int    behindText( SdPoint p, SdPoint &org, QString &dest, SdPropText &prop );
    virtual bool   getInfo( SdPoint p, QString &info, bool extInfo );
    //Определение разумной точки
    virtual bool   snapPoint( SdSnapInfo *snap );

    friend class SdSelector;
  };

typedef SdGraph *SdGraphPtr;

#endif // SDGRAPHOBJECT_H
