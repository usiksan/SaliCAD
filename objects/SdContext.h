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

class SdConverter;
class SdSelector;

//Контекст вывода графической информации (средство рисования)
class SdContext {
    SdConverter    *mConverter;    //Преобразователь координат
    SdPoint         mGrid;         //Сетка
    SdSelector     *mSelector;     //Объект-селектор относительно которого выполняется рисование
  protected:
    SdPoint         convertPoint( SdPoint sour );
    int             convertCoord( int sour );
    SdAngle         convertAngle( SdAngle sour );
    int             convertLayer( int layer );
    int             convertMirror( int mirror );
    bool            isLayerVisible( int layer );   //Истина если слой виден (с учетом преобразования)
    int             convertBaseLayer( int layer ); //Возвращает слой после преобразования
  public:
    SdContext( SdPoint grid );

    //Операции с преобразователем координат
    SdContext*      setConverter( SdConverter *c );
    void            removeConverter( SdConverter *c );

    //Операции с селектором
    void            setSelector( SdSelector* );
    SdSelector     *getSelector() { return mSelector; }

    //Информационные
            SdPoint getGrid() const { return mGrid; }
    virtual void    getViewPort( SdRect *dest ) = 0;

    //Примитивы рисования
    virtual void    line( SdPoint a, SdPoint b, SdLineProp &prop ) = 0;
    virtual void    road( SdPoint a, SdPoint b, int clear, SdRoadProp &prop ) = 0;
            void    rect( SdQuadrangle q, SdLineProp &prop );
    virtual void    fillRect( SdPoint a, SdPoint b, SdLineProp &prop ) = 0;
    virtual void    arc( SdPoint center, int radius, SdAngle start, SdAngle stop, SdLineProp &prop ) = 0;
    virtual void    circle( SdPoint center, int radius, SdLineProp &prop ) = 0;
    virtual void    text( const QString str, SdTextProp &prop, bool special = false ) = 0;
    //virtual void   VectorText( DTextProp &prop, CPChar str, bool special = false ) = 0;
            void    region( SdPointList &points, SdLineProp &prop, bool autoClose = true );
    virtual void    polygon( SdPointList &points, SdPolyWindowList &windows, SdRoadProp &prop ) = 0;
    //virtual void TextBox( DTextProp &prop, int charNum );

    //Примитивы второго уровня
    virtual void    smartPoint( SdPoint a, int smartMask = snapCommon ) = 0;      //Отобразить точку привязки
    virtual void    dotPoint( SdPoint p, SdLineProp &prop ) = 0;                  //Точка пересечения цепей
    virtual void    drawCross( SdPoint p, SdSymPinProp &prop ) = 0;               //Точка перекрестья вывода символа
    virtual void    dotCircle( SdPoint p, SdPartPinProp &prop ) = 0;              //Точка подключения вывода корпуса
    virtual void    drawArrow( SdPoint p, SdAngle angle, SdLineProp &prop ) = 0;  //Размерная стрелка
    virtual void    via( SdPoint p, SdRoadProp &prop ) = 0;                       //Переходное (без КП)
    virtual void    dotTrase( SdPoint p );                                      //Точка пометки трассируемой цепи
  };




#endif // SDCONTEXT_H
