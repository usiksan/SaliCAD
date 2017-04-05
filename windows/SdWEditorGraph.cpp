/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdWEditorGraph.h"
#include "SdWView.h"
#include "objects/SdContext.h"
#include "objects/SdEnvir.h"
#include "objects/SdConverterView.h"
#include "objects/SdGraph.h"
#include "modes/SdMode.h"
#include <QPainter>
#include <QPaintEvent>

SdWEditorGraph::SdWEditorGraph(QWidget *parent) :
  SdWEditor( parent ),
  mView(0),           //Порт для отображения объекта
  mMode(0),           //Режим для исполнения операций
  mPrevMode(0),       //Предыдущий режим
  mStack(0),          //Временный режим
  mSelect(0),         //Режим выделения
  mScale(),           //Текущий масштаб изображения
  mOrigin(),          //Логическая точка центра изображения
  mClientSize(),      //Размер клиентской области в пикселах
  mGrid(),            //Размер сетки
  mLeftDown(false),   //Флаг нажатия левой кнопки мыши
  mDrag(false),       //Флаг активного режима перетаскивания
  mPpm(10.0),         //Логических единиц в мм (преобразование в реальные размеры)
  mDownPoint(),       //Точка нажатия левой кнопки мыши
  mPrevPoint(),       //Предыдущая точка перемещения мыши
  mLastOver(),        //Последний охватывающий прямоугольник
  mScrollSizeX(1.0),  //Размер скроллинга на еденицу прокрутки
  mScrollSizeY(1.0),  //Размер скроллинга на еденицу прокрутки
  mPixelTransform()   //Pixel to phys transformation
  {
  mView = new SdWView();
  setViewport( mView );
  }




//Temporary call mode
void SdWEditorGraph::modeCall(SdModeTemp *mode)
  {
  setFocus();
  Q_ASSERT( mode != nullptr );
  //Стековый режим. Прежний стековый режим снести
  if( mStack ) delete mStack;
  mode->setMainMode( mMode );
  mode->reset();
  modeActivate( mStack = mode );
  }




//Set new mode
void SdWEditorGraph::modeSet(SdMode *mode)
  {
  //Установка режима
  //Стековый режим - снести совсем
  //Прежний режим - снести (если выделения - то просто затереть, иначе - для обычного режима
  //снести совсем, а для выделения перенести в прежний
  setFocus();
  Q_ASSERT( mode != nullptr );
  if( mStack ) {
    //Стековый режим снести совсем
    delete mStack;
    mStack = 0;
    }
  //Если был режим выделения - просто затереть
  if( mMode && mMode == mSelect ) mMode = mode;
  else {
    if( mode == mSelect ) {
      //Если новый режим выделения - то старый передать в prevMode
      if( mPrevMode ) delete mPrevMode;
      mPrevMode = mMode;
      mMode = mode;
      }
    else {
      //Иначе - снести предыдущий режим
      delete mMode;
      mMode = mode;
      }
    }
  if( mMode ) mMode->reset();
  modeActivate( mode );
  }




//Cancel current mode and restore previous
void SdWEditorGraph::modeCancel()
  {
  //Завершение режима.
  if( mStack ) {
    //Если был стековый режим, то снести и реанимировать текущий режим
    delete mStack;
    mStack = 0;
    modeRestore( mMode );
    }
  else {
    //Завершение текущего режима
    if( mMode == mSelect ) {
      //Если текущим был режим выделения, то активировать прежний, если он был
      if( mPrevMode ) {
        mMode = mPrevMode;
        mPrevMode = 0;
        modeActivate( mMode );
        }
      }
    else {
      //Если текущим был обычный режим, то перенести его в предыдущий
      if( mPrevMode ) delete mPrevMode;
      mPrevMode = mMode;
      //и установить режим выделения
      modeActivate( mMode = mSelect );
      }
    }
  }




void SdWEditorGraph::paintEvent(QPaintEvent *event)
  {
  event->accept();
  if( viewport() == nullptr ) return;
  QSize s = viewport()->size();
  QPainter painter( viewport() );

  SdContext context( mGrid, &painter );
  SdConverterView cv( &context, s, mOrigin, mScale.getScale() );

  mPixelTransform = cv.getMatrix().inverted();

  //Очистить фон
  painter.fillRect( 0,0, s.width(), s.height(), sdEnvir->getSysColor(scGraphBack) );

  //Рисовать сетку
  if( sdEnvir->mGridView ) {
    //Не чертить сетку менее чем в minViewGrid пикселов
    if( mScale.phys2pixel(mGrid.x()) >= sdEnvir->mMinViewGrid && mScale.phys2pixel(mGrid.y()) >= sdEnvir->mMinViewGrid ) {
      //Grid color
      painter.setPen( sdEnvir->getSysColor(scGrid) );
      SdPoint tmp( mPixelTransform.map(QPoint(0,s.height())) );
      tmp.setX( (tmp.x() / mGrid.x() ) * mGrid.x() );
      tmp.setY( (tmp.y() / mGrid.y() ) * mGrid.y() );
      int oldx = tmp.x();
      QPoint p = context.transform().map( tmp );
      while( p.y() > 0 ) {
        while( p.x() < s.width() ) {
          painter.drawPoint(tmp);
          tmp.setX( tmp.x() + mGrid.x() );
          p = context.transform().map( tmp );
          }
        tmp.setX( oldx );
        tmp.setY( tmp.y() + mGrid.y() );
        p = context.transform().map( tmp );
        }
      }
    }
  //Рисовать содержимое
  if( modeGet() )
    modeGet()->draw( &context );
  else {
    //Рисовать содержимое
    getProjectItem()->draw( &context );
    //TODO ratNet
    //GetObject()->ForEach( DrawIterator( wc, 0, GetEnvir().showRatNet ) );
    }
  //Рисовать курсор
  //TODO cursor
  //wc.DrawHideCursor( prevPoint );
  viewport()->setCursor( Qt::PointingHandCursor );
  }




//Activate new mode
void SdWEditorGraph::modeActivate(SdMode *mode)
  {
  if( mode ) mode->activate();
  }

void SdWEditorGraph::modeRestore(SdMode *mode)
  {

  }

