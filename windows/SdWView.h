/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDWVIEW_H
#define SDWVIEW_H

#include "objects/SdPoint.h"
#include "objects/SdRect.h"
#include "objects/SdScaler.h"
#include "modes/SdMode.h"
#include <QWidget>

class SdWView : public QWidget
  {
    Q_OBJECT

    //SdProjectItem *mItem;
    SdMode   *mMode;           //Режим для исполнения операций
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
  public:
    explicit SdWView(QWidget *parent = 0);

  signals:

  public slots:
  };

#endif // SDWVIEW_H
