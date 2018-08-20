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
#include <QPrinter>


class SdModeSelect;

class SdWEditorGraph : public SdWEditor
  {
    Q_OBJECT

  protected:
    SdWView         *mView;           //Port for object view [Порт для отображения объекта]
    SdMode          *mMode;           //Current active mode [Режим для исполнения операций]
    SdMode          *mPrevMode;       //Previous mode [Предыдущий режим]
    SdMode          *mStack;          //Temporary mode [Временный режим]
    SdModeSelect    *mSelect;         //Select mode [Режим выделения]
    SdScaler         mScale;          //Текущий масштаб изображения
    SdPoint          mOrigin;         //Логическая точка центра изображения
    SdPoint          mClientSize;     //Размер клиентской области в пикселах
    SdPoint          mGrid;           //Размер сетки
    bool             mLeftDown;       //Флаг нажатия левой кнопки мыши
    bool             mDrag;           //Флаг активного режима перетаскивания
    SdPoint          mDownPoint;      //Точка нажатия левой кнопки мыши
    SdPoint          mPrevPoint;      //Предыдущая точка перемещения мыши
    SdPoint          mPrevEnter;      //Предыдущая точка нажатия левой кнопки мыши
    SdRect           mLastOver;       //Последний охватывающий прямоугольник
    double           mScrollSizeX;    //Размер скроллинга на еденицу прокрутки
    double           mScrollSizeY;    //Размер скроллинга на еденицу прокрутки
    QTransform       mPixelTransform; //Pixel to phys transformation
    bool             mCasheDirty;     //Cashe dirty flag. When true - static part redrawn
    QImage           mCashe;          //Cashe for static picture part

  public:
    SdWEditorGraph( SdProjectItem *item, QWidget *parent = nullptr );

    double  scaleGet() const { return mScale.scaleGet(); }
    void    scaleSet( double scale );
    void    scaleStep( double step );

    SdPoint originGet() const { return mOrigin; }
    void    originSet( SdPoint org );

    SdPoint gridGet() const { return mGrid; }
    void    gridSet( SdPoint grid );

    SdPoint clientSize() const { return mClientSize; }

    //Return current cursor point coord in logical
    SdPoint cursorPosition() const { return mPrevPoint; }

    //Declare paint area as dirty. This rebuild static paint area when next draw
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
    double  getPPM() const;

    //Handle move cursor
    void    cursorMove( int dx, int dy );

    //Commands
    virtual void           cmFilePrint() override;

    virtual void           cmEditUndo() override;
    virtual void           cmEditRedo() override;
    virtual void           cmEditCut() override;
    virtual void           cmEditCopy() override;
    virtual void           cmEditPaste() override;
    virtual void           cmEditDelete() override;
    virtual void           cmEditSelectAll() override;
    virtual void           cmEditUnSelect() override;
//    virtual void cmEditFind() {}
//    virtual void cmEditReplace() {}
//    virtual void cmEditProperties() {}
    virtual void           cmClipboardChange() override;

    virtual void           onActivateEditor() override;
    virtual void           cmViewFit() override;
    virtual void           cmViewGrid() override;
    virtual void           cmViewLayers() override;
    virtual void           cmViewZoomIn() override;
    virtual void           cmViewZoomOut() override;
    virtual void           cmViewWindow() override;
    virtual void           cmViewMeasurement() override;

    virtual void           cmPropChanged() override;

    virtual void           cmModeLine() override;
    virtual void           cmModeRect() override;
    virtual void           cmModeFilledRect() override;
    virtual void           cmModeRegion() override;
//    virtual void cmModeFilledRegion() {}
    virtual void           cmModeCircle() override;
//    virtual void cmModeFilledCircle() {}
    virtual void           cmModeArc() override;
    virtual void           cmModeText() override;
//    virtual void cmModeField() {}
    virtual void           cmModeSelect() override;

    virtual void           cmEnterPosition() override;

  public slots:

    //Print dialog
    void    printDialog( SdRect wnd );


  protected:
    //Print projectItem or selection in desired window
    void    print(QPrinter &printer, SdRect wnd, int zeroWidth, bool colorPrint, double scale, SdSelector *selector );

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
    virtual void resizeEvent(QResizeEvent *event) override;

  };

#endif // SDWEDITORGRAPH_H
