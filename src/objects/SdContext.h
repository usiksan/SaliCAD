﻿/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Context for graphics draw
*/

#ifndef SDCONTEXT_H
#define SDCONTEXT_H

#include "SdPoint.h"
#include "SdPropAngle.h"
#include "SdRect.h"
#include "SdProp.h"
#include "SdPointList.h"
#include "SdQuadrangle.h"
#include "SdPolyWindowList.h"
#include "SdSnapInfo.h"
#include "SdScaler.h"

#include <QPainter>
#include <QTransform>

class SdConverter;
class SdSelector;

//Контекст вывода графической информации (средство рисования)
class SdContext {
  protected:
    QPainter       *mPainter;      //Рисовальщик
    SdConverter    *mConverter;    //Преобразователь координат
    SdPoint         mGrid;         //Сетка
    SdSelector     *mSelector;     //Объект-селектор относительно которого выполняется рисование
    QTransform      mTransform;    //Transform matrix
    bool            mMirror;       //Mirror flag
    int             mAngle;        //Rotation angle
    SdScaler        mScaler;       //Current scale
    bool            mPairLayer;    //True for paired layer
    QColor          mOverColor;    //Color for overriding default layers color
    bool            mOverOn;       //True if overriding is on
    int             mZeroWidth;    //Width for zero width line
    bool            mZeroOn;       //True if overriding zero width line on
  public:
    SdContext(SdPoint grid, QPainter *painter);
    virtual ~SdContext() {}

    //Operations with coord convertor
    SdContext*      setConverter( SdConverter *c );
    void            removeConverter( SdConverter *c );

    //Overriding color. If this color is setuped then drawing perform by this color but not default layer color.
    //This possibility is for draw "selected" elements or "enter" elements and so on
    //By default over color is not defined
    void            setOverColor( QColor color ) { mOverColor = color; mOverOn = true; }
    void            resetOverColor() { mOverOn = false; }


    //Overriding zero width line. If this width is setupedthen drawing perform with this width when draw zero width line.
    //This possibility is for print. By default over width is not defined
    void            setOverZeroWidth( int width ) { mZeroWidth = width; mZeroOn = true; }
    void            resetOverZeroWidth() { mZeroOn = false; }

    //Operation with selector [Операции с селектором]
    void            setSelector( SdSelector *selector );
    SdSelector     *getSelector() { return mSelector; }

    //Information [Информационные]
    SdPoint         getGrid() const { return mGrid; }
    QTransform&     transform() { return mTransform; }

    //Draw prepare
    void            setPen(int width, SdLayer *layer, int lineStyle);
    virtual void    setPen(int width, QColor color, int lineStyle );
    void            setBrush( QColor color );
    void            setProp(const SdPropLine &prop );
    void            setFont( const SdPropText &prop );

    //Drawing primitives [Примитивы рисования]
    virtual void    line( SdPoint a, SdPoint b );
    void            line(SdPoint a, SdPoint b, const SdPropLine &prop );
    void            quadrangle( SdQuadrangle q, const SdPropLine &prop );
    void            rect( SdRect r );
    void            rect( SdRect r, const SdPropLine &prop );
    virtual void    fillRect( SdRect r );
    void            fillRect( SdRect r, const SdPropLine &prop );
    virtual void    arc( SdPoint center, SdPoint start, SdPoint stop );
    void            arc( SdPoint center, SdPoint start, SdPoint stop, const SdPropLine &prop );
    virtual void    circle( SdPoint center, int radius );
    void            circle( SdPoint center, int radius, const SdPropLine &prop );
    virtual void    circleFill( SdPoint center, int radius );
    void            circleFill( SdPoint center, int radius, const SdPropLine &prop );
    void            textEx(SdPoint pos, SdRect &over, const QString str, int dir, int horz, int vert, int cursor = 0, SdPoint *cp1 = nullptr, SdPoint *cp2 = nullptr, SdRect *sel = nullptr, int start = 0, int stop = 0);
    void            text( SdPoint pos, SdRect &over, const QString str, const SdPropText &prop );
    void            region( const SdPointList &points, bool autoClose = true );
    void            region( const SdPointList &points, const SdPropLine &prop, bool autoClose = true );
    virtual void    regionFill( const SdPointList &points, const SdPropLine &prop );
    virtual void    polygon( const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer );

    //Second level primitives [Примитивы второго уровня]
    void            smartPoint(SdPoint a, SdSnapMask smartMask = snapCommon );      //Отобразить точку привязки
    void            symPin( SdPoint a, SdLayer *layer );                      //Symbol pin cross
    void            partPin( SdPoint a, SdLayer *layer );                     //Part pin circle
    void            cross( SdPoint a, int size, QColor color );               //Draw cross
    void            drawLineArrow( SdPoint p1, SdPoint p2, QColor color, int arrowSize );  //Размерная стрелка

    //Draw cursor with current mode
    void            drawCursor( SdPoint p );

  private:

    //Update converter context
    void     updateConverter();

    //Convert color
    QColor   convertColor( SdLayer *layer );

    //Convert layer
    SdLayer *convertLayer( SdLayer *layer ) { return mPairLayer ? layer->pair() : layer; }

    //Check - is there layer visible
    bool     isVisible( SdLayer *layer ) { return convertLayer(layer)->isVisible(); }
  };




#endif // SDCONTEXT_H