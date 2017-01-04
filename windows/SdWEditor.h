/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDWEDITOR_H
#define SDWEDITOR_H


#include "objects/SdPoint.h"
#include "objects/SdRect.h"
#include "objects/SdScaler.h"
#include <QWidget>
#include <QString>

class SdProjectItem;

class SdWEditor : public QWidget
  {
    Q_OBJECT

  protected:
    SdScaler  mScale;          //Текущий масштаб изображения
    SdPoint   mOrigin;         //Логическая точка центра изображения
    SdPoint   mClientSize;     //Размер клиентской области в пикселах
    SdPoint   mGrid;           //Размер сетки
    bool      mLeftDown;       //Флаг нажатия левой кнопки мыши
    bool      mDrag;           //Флаг активного режима перетаскивания
    double    mPpm;            //Логических единиц в мм (преобразование в реальные размеры)
    SdPoint   mDownPoint;      //Точка нажатия левой кнопки мыши
    SdPoint   mPrevPoint;      //Предыдущая точка перемещения мыши
    SdRect    mLastOver;       //Последний охватывающий прямоугольник
    double    mScrollSizeX;    //Размер скроллинга на еденицу прокрутки
    double    mScrollSizeY;    //Размер скроллинга на еденицу прокрутки

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
    explicit SdWEditor(QWidget *parent = 0);

    virtual SdProjectItem* getProjectItem() = 0;

    QString                getTitle();

    QString                getIconName();

    //Реакции на команды
    virtual void cmFilePrint() {}

    virtual void cmFileImport() {}
    virtual void cmFileExport() {}

    virtual void cmEditUndo() {}
    virtual void cmEditRedo() {}
    virtual void cmEditCut() {}
    virtual void cmEditCopy() {}
    virtual void cmEditPaste() {}
    virtual void cmEditDelete() {}
    virtual void cmEditSelectAll() {}
    virtual void cmEditFind() {}
    virtual void cmEditReplace() {}
    virtual void cmEditProperties() {}

    virtual void cmViewProject() {}
    virtual void cmViewFill() {}
    virtual void cmViewNets() {}
    virtual void cmViewGrid() {}
    virtual void cmViewZoomIn() {}
    virtual void cmViewZoomOut() {}
    virtual void cmViewArea() {}
    virtual void cmViewMeasurement() {}

    virtual void cmModeLine() {}
    virtual void cmModeRect() {}
    virtual void cmModeFilledRect() {}
    virtual void cmModeRegion() {}
    virtual void cmModeFilledRegion() {}
    virtual void cmModeCircle() {}
    virtual void cmModeFilledCircle() {}
    virtual void cmModeArc() {}
    virtual void cmModeText() {}
    virtual void cmModeField() {}

    //3d
    virtual void cmBall() {}
    virtual void cmPinWired() {}
    virtual void cmPinFlat() {}
    virtual void cmBodyCylinder() {}
    virtual void cmBodyBrick() {}
    virtual void cmBodyContur() {}

    //Symbol Part
    virtual void cmModePin() {}
    virtual void cmModeReference() {}
    virtual void cmModeOrigin() {}

    //Sheet
    virtual void cmModeComponent() {}
    virtual void cmModeNet() {}
    virtual void cmNetSetup() {}
    virtual void cmModeBus() {}
    virtual void cmModeDisconnect() {}
    virtual void cmModePcbArea() {}
    virtual void cmModeLink() {}
    virtual void cmModeNetName() {}
    virtual void cmModeNetList() {}
    virtual void cmModePack() {}

    //Pcb
    virtual void cmPads() {}
    virtual void cmModeLineSize() {}
    virtual void cmModeRadiusSize() {}
    virtual void cmModeMovePart() {}
    virtual void cmModePlace() {}

    virtual void cmModeEditWire() {}
    virtual void cmModeWire() {}
    virtual void cmModePolygon() {}
    virtual void cmModeDeleteWire() {}
    virtual void cmModePad() {}

    virtual void cmOption() {}
    virtual void cmTools() {}

    virtual void cmHelpContens() {}
    virtual void cmHelpIndex() {}
    virtual void cmHelpAbout() {}

    virtual void cmClipboardChange() {}

  signals:

  public slots:

    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *) override;
  };

#endif // SDWEDITOR_H
