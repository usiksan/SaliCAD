/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphics editor, common part
*/

#ifndef SDWEDITORGRAPH_H
#define SDWEDITORGRAPH_H

#include "SdWEditor.h"
#include "SdWView.h"
#include "modes/SdMode.h"
#include "modes/SdModeTemp.h"

#include <QTransform>

class SdWEditorGraph : public SdWEditor
  {
    Q_OBJECT

  protected:
    SdWView   *mView;           //Порт для отображения объекта
    SdMode    *mMode;           //Режим для исполнения операций
    SdMode    *mPrevMode;       //Предыдущий режим
    SdMode    *mStack;          //Временный режим
    SdMode    *mSelect;         //Режим выделения
    SdScaler   mScale;          //Текущий масштаб изображения
    SdPoint    mOrigin;         //Логическая точка центра изображения
    SdPoint    mClientSize;     //Размер клиентской области в пикселах
    SdPoint    mGrid;           //Размер сетки
    bool       mLeftDown;       //Флаг нажатия левой кнопки мыши
    bool       mDrag;           //Флаг активного режима перетаскивания
    double     mPpm;            //Логических единиц в мм (преобразование в реальные размеры)
    SdPoint    mDownPoint;      //Точка нажатия левой кнопки мыши
    SdPoint    mPrevPoint;      //Предыдущая точка перемещения мыши
    SdRect     mLastOver;       //Последний охватывающий прямоугольник
    double     mScrollSizeX;    //Размер скроллинга на еденицу прокрутки
    double     mScrollSizeY;    //Размер скроллинга на еденицу прокрутки
    QTransform mPixelTransform; //Pixel to phys transformation

//    int    ScaleDCoord( DCoord sour );         //Преобразовать логическую координату в экранную
//    NPoint ScaleDPoint( DPoint p );            //Преобразовать логическую точку в экранную
//    DPoint ConvertPoint( LPARAM lParam );      //Преобразовать экранную точку в логическую
//    DCoord Convert( int i );                   //Преобразовать экранную координату в логическую
//    void   AssignGraphBar( NControlBar *bar ); //Добавляет стандартную панель инструментов
//    int    ConvertKeyCode( WPARAM wParam );    //Преобразование кодов клавиш в специальные коды

    bool   editedFlag;  //Флаг состояния редактирования проекта
    bool   bDirty;      //Флаг редактирования объекта
    bool   emptyUndo;   //Флаг состояния буфера отмены

  public:
    SdWEditorGraph( QWidget *parent = 0 );

    //Temporary call mode
    void    modeCall( SdModeTemp *mode );

    //Set new mode
    void    modeSet( SdMode *mode );

    //Cancel current mode and restore previous
    void    modeCancel();

    //Get active mode
    SdMode *modeGet() { return mStack == nullptr ? mMode : mStack; }

    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event);

    //Activate new mode
    void modeActivate( SdMode *mode );

    //Restore previous mode
    void modeRestore( SdMode *mode );
  };

#endif // SDWEDITORGRAPH_H
