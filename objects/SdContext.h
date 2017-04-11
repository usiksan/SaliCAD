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
            SdPoint getGrid() const { return mGrid; }
    QTransform&     transform() { return mTransform; }
    //virtual void    getViewPort( SdRect *dest ) = 0;

    //Примитивы рисования
    void            line( SdPoint a, SdPoint b, SdPropLine &prop );
    //virtual void    road( SdPoint a, SdPoint b, int clear, SdRoadProp &prop ) = 0;
            void    rect( SdQuadrangle q, SdPropLine &prop );
            void    rectSelect(SdRect r, QColor color );
//    virtual void    fillRect( SdPoint a, SdPoint b, SdPropLine &prop ) = 0;
//    virtual void    arc( SdPoint center, int radius, SdAngle start, SdAngle stop, SdPropLine &prop ) = 0;
//    virtual void    circle( SdPoint center, int radius, SdPropLine &prop ) = 0;
//    virtual void    text( const QString str, SdTextProp &prop, bool special = false ) = 0;
    //virtual void   VectorText( DTextProp &prop, CPChar str, bool special = false ) = 0;
            void    region( SdPointList &points, SdPropLine &prop, bool autoClose = true );
//    virtual void    polygon( SdPointList &points, SdPolyWindowList &windows, SdRoadProp &prop ) = 0;
    //virtual void TextBox( DTextProp &prop, int charNum );

    //Примитивы второго уровня
//    virtual void    smartPoint( SdPoint a, int smartMask = snapCommon ) = 0;      //Отобразить точку привязки
//    virtual void    dotPoint( SdPoint p, SdPropLine &prop ) = 0;                  //Точка пересечения цепей
//    virtual void    drawCross( SdPoint p, SdSymPinProp &prop ) = 0;               //Точка перекрестья вывода символа
//    virtual void    dotCircle( SdPoint p, SdPartPinProp &prop ) = 0;              //Точка подключения вывода корпуса
//    virtual void    drawArrow( SdPoint p, SdAngle angle, SdPropLine &prop ) = 0;  //Размерная стрелка
//    virtual void    via( SdPoint p, SdRoadProp &prop ) = 0;                       //Переходное (без КП)
    void            dotTrase( SdPoint p );                                      //Точка пометки трассируемой цепи

  private:
    void     setPen(int width, SdLayer *layer );

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
