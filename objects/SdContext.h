/*
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
#include "SdAngle.h"
#include "SdRect.h"
#include "SdProp.h"
#include "SdPointList.h"
#include "SdQuadrangle.h"
#include "SdPolyWindowList.h"
#include "SdSnapInfo.h"

#include <QPainter>
#include <QTransform>

class SdConverter;
class SdSelector;

//Контекст вывода графической информации (средство рисования)
class SdContext {
    QPainter       *mPainter;      //Рисовальщик
    SdConverter    *mConverter;    //Преобразователь координат
    SdPoint         mGrid;         //Сетка
    SdSelector     *mSelector;     //Объект-селектор относительно которого выполняется рисование
    QTransform      mTransform;    //Transform matrix
    bool            mPairLayer;    //True for paired layer
  public:
    SdContext(SdPoint grid, QPainter *painter);

    //Операции с преобразователем координат
    SdContext*      setConverter( SdConverter *c );
    void            removeConverter( SdConverter *c );

    //Операции с селектором
    void            setSelector( SdSelector* );
    SdSelector     *getSelector() { return mSelector; }

    //Информационные
    SdPoint         getGrid() const { return mGrid; }
    QTransform&     transform() { return mTransform; }
    //virtual void    getViewPort( SdRect *dest ) = 0;

    //Draw prepare
    void            setPen(int width, SdLayer *layer, int lineStyle);
    void            setPen(int width, QColor color, int lineStyle );
    void            setBrush( QColor color );
    void            setProp(const SdPropLine &prop );
    void            setFont( const SdPropText &prop );

    //Примитивы рисования
    void            line( SdPoint a, SdPoint b );
    void            line(SdPoint a, SdPoint b, const SdPropLine &prop );
    //virtual void    road( SdPoint a, SdPoint b, int clear, SdRoadProp &prop ) = 0;
    void            quadrangle( SdQuadrangle q, const SdPropLine &prop );
    void            rect( SdRect r );
    void            rect( SdRect r, const SdPropLine &prop );
    void            fillRect( SdRect r );
    void            fillRect( SdRect r, const SdPropLine &prop );
//    virtual void    arc( SdPoint center, int radius, SdAngle start, SdAngle stop, SdPropLine &prop ) = 0;
    void            circle( SdPoint center, int radius );
    void            circle( SdPoint center, int radius, const SdPropLine &prop );
    void            circleFill( SdPoint center, int radius );
    void            circleFill( SdPoint center, int radius, const SdPropLine &prop );
    void            textEx(SdPoint pos, SdRect &over, const QString str, int dir, int horz, int vert, int cursor = 0, SdPoint *cp1 = 0, SdPoint *cp2 = 0, SdRect *sel = 0, int start = 0, int stop = 0);
    void            text( SdPoint pos, SdRect &over, const QString str, const SdPropText &prop );
    void            region( const SdPointList &points, bool autoClose = true );
    void            region( const SdPointList &points, const SdPropLine &prop, bool autoClose = true );
//    virtual void    polygon( SdPointList &points, SdPolyWindowList &windows, SdRoadProp &prop ) = 0;
    //virtual void TextBox( DTextProp &prop, int charNum );

    //Примитивы второго уровня
    void            smartPoint( SdPoint a, int smartMask = snapCommon );      //Отобразить точку привязки
    void            symPin( SdPoint a, SdLayer *layer );                      //Symbol pin cross
    void            partPin( SdPoint a, SdLayer *layer );                     //Part pin circle
    void            cross( SdPoint a, int size, QColor color );               //Draw cross
//    virtual void    dotPoint( SdPoint p, SdPropLine &prop ) = 0;                  //Точка пересечения цепей
//    virtual void    dotCircle( SdPoint p, SdPartPinProp &prop ) = 0;              //Точка подключения вывода корпуса
//    virtual void    drawArrow( SdPoint p, SdAngle angle, SdPropLine &prop ) = 0;  //Размерная стрелка
//    virtual void    via( SdPoint p, SdRoadProp &prop ) = 0;                       //Переходное (без КП)
    void            dotTrase( SdPoint p );                                      //Точка пометки трассируемой цепи

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
