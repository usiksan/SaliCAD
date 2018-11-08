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
#include "SdPropSelected.h"

class SdSelector;
class SdContext;
class SdSnapInfo;
class QWidget;
class SdProjectItem;

class SdGraph : public SdObject
  {
  protected:
    SdSelector *mSelector;
  public:
    SdGraph();

    //Select object
    SdSelector*    getSelector() { return mSelector; }
    bool           isSelected() const { return mSelector != nullptr; }

    //Save object state
    virtual void   saveState( SdUndo *undo );
    virtual void   moveComplete( SdPoint grid, SdUndo *undo );

    //Изменение перемещением
    virtual void   move( SdPoint offset );
    virtual void   rotate( SdPoint center, SdPropAngle angle );
    virtual void   mirror( SdPoint a, SdPoint b );

    //Изменение свойствами
    virtual void   setProp( SdPropSelected &prop );
    virtual void   getProp( SdPropSelected &prop );

    //Изменение обновлением текста
    virtual void   setText(int index, QString sour, SdPropText &prop, QWidget *parent );

    //Варианты выделения
    virtual void   selectByPoint( const SdPoint p, SdSelector *selector );
    virtual void   selectByRect( const SdRect &r, SdSelector *selector );
    virtual void   select( SdSelector *selector );
    virtual void   prepareMove( SdUndo *undo );
    //Layer maintance
    virtual void   setLayerUsage();
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
    //Find snap point on object
    virtual void   snapPoint( SdSnapInfo *snap );

    friend class SdSelector;
  };

typedef SdGraph *SdGraphPtr;

#endif // SDGRAPHOBJECT_H
