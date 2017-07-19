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
#include <QMenu>

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
    QPoint     mCursorPos;      //Позиция курсора в координатах экрана
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
    SdWEditorGraph( SdProjectItem *item, QWidget *parent = 0 );

    double  scaleGet() const { return mScale.scaleGet(); }
    void    scaleSet( double scale );
    void    scaleStep( double step );

    SdPoint originGet() const { return mOrigin; }
    void    originSet( SdPoint org );

    SdPoint gridGet() const { return mGrid; }
    void    gridSet( SdPoint grid );

    SdPoint clientSize() const { return mClientSize; }

    void    dirtyCashe() { mCasheDirty = true; }

    //Update selection status
    void    setSelectionStatus( bool status );

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

    //Show and exec context menu
    void    contextMenu( QMenu *menu );

    //return ppm for this editor. PPM is how much phys in one logical
    virtual double         getPPM() const = 0;

    //Commands
    virtual void           cmEditUndo() override;
    virtual void           cmEditRedo() override;
//    virtual void           cmEditCut() override;
//    virtual void cmEditCopy() {}
//    virtual void cmEditPaste() {}
//    virtual void cmEditDelete() {}
//    virtual void cmEditSelectAll() {}
//    virtual void cmEditFind() {}
//    virtual void cmEditReplace() {}
//    virtual void cmEditProperties() {}
    virtual void           cmClipboardChange() override;

    virtual void           onActivateEditor() override;
    virtual void           cmViewFit() override;
    virtual void           cmViewZoomIn() override;
    virtual void           cmViewZoomOut() override;
    virtual void           cmViewWindow() override;

    virtual void           cmPropChanged() override;

    virtual void           cmModeLine() override;
    virtual void           cmModeRect() override;
    virtual void           cmModeFilledRect() override;
    virtual void           cmModeRegion() override;
//    virtual void cmModeFilledRegion() {}
//    virtual void cmModeCircle() {}
//    virtual void cmModeFilledCircle() {}
    virtual void           cmModeArc() override;
    virtual void           cmModeText() override;
//    virtual void cmModeField() {}
    virtual void           cmModeSelect() override;

    virtual void           cmEnterPosition() override;

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
