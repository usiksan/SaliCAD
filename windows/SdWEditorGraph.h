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
    SdPoint    mDownPoint;      //Точка нажатия левой кнопки мыши
    SdPoint    mPrevPoint;      //Предыдущая точка перемещения мыши
    SdRect     mLastOver;       //Последний охватывающий прямоугольник
    double     mScrollSizeX;    //Размер скроллинга на еденицу прокрутки
    double     mScrollSizeY;    //Размер скроллинга на еденицу прокрутки
    QTransform mPixelTransform; //Pixel to phys transformation
    bool       mCasheDirty;     //Cashe dirty flag. When true - static part redrawn
    QImage     mCashe;          //Cashe for static picture part

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

    double  scaleGet() const { return mScale.scaleGet(); }
    void    scaleSet( double scale );
    void    scaleStep( double step );

    SdPoint originGet() const { return mOrigin; }
    void    originSet( SdPoint org );

    SdPoint clientSize() const { return mClientSize; }

    //Temporary call mode
    void    modeCall( SdModeTemp *mode );

    //Set new mode
    void    modeSet( SdMode *mode );

    //Cancel current mode and restore previous
    void    modeCancel();

    //Get active mode
    SdMode *modeGet() { return mStack == nullptr ? mMode : mStack; }

    //Window zoom
    void    zoomWindow( SdRect r );

    //return ppm for this editor. PPM is how much phys in one logical
    virtual double         getPPM() const = 0;

    //Commands
    virtual void           onActivateEditor() override;
    virtual void           cmViewFit() override;
    virtual void           cmViewZoomIn() override;
    virtual void           cmViewZoomOut() override;
    virtual void           cmViewWindow() override;

  protected:
    //Activate new mode
    void    modeActivate( SdMode *mode );

    //Restore previous mode
    void    modeRestore( SdMode *mode );

    //Display cursor positions from mPrevPoint
    void    displayCursorPositions();

    //Get phys coord from pixel
    SdPoint pixel2phys( QPoint pos );

    //Setup mouse pos. Where pos is pixel coord
    void    updateMousePos(QMouseEvent *event);


    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

  };

#endif // SDWEDITORGRAPH_H
