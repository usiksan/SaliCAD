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
#include "objects/SdPulsar.h"
#include "modes/SdMode.h"
#include "modes/SdModeTZoomer.h"

#include <QPainter>
#include <QPaintEvent>
#include <QImage>
#include <QtDebug>

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
  mDownPoint(),       //Точка нажатия левой кнопки мыши
  mPrevPoint(),       //Предыдущая точка перемещения мыши
  mLastOver(),        //Последний охватывающий прямоугольник
  mScrollSizeX(1.0),  //Размер скроллинга на еденицу прокрутки
  mScrollSizeY(1.0),  //Размер скроллинга на еденицу прокрутки
  mPixelTransform(),  //Pixel to phys transformation
  mCasheDirty(true),  //Cashe dirty flag. When true - static part redrawn
  mCashe()            //Cashe for static picture part
  {
  mView = new SdWView();
  setViewport( mView );
  setMouseTracking(true);
  }

void SdWEditorGraph::scaleStep(double step)
  {
  mScale.scale(step);
  mCasheDirty = true;
  update();
  }




void SdWEditorGraph::originSet(SdPoint org)
  {
  mOrigin = org;
  mCasheDirty = true;
  update();
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




void SdWEditorGraph::cmViewZoomIn()
  {
  modeCall( new SdModeTZoomer(true, this, getProjectItem() ) );
  }


void SdWEditorGraph::cmViewZoomOut()
  {
  modeCall( new SdModeTZoomer(false, this, getProjectItem() ) );
  }


void SdWEditorGraph::paintEvent(QPaintEvent *event)
  {
  event->accept();
  if( viewport() == nullptr ) return;
  QSize s = viewport()->size();

  if( mCasheDirty ) {
    //Redraw static part
    mCashe = QImage( s, QImage::Format_ARGB32_Premultiplied );
    QPainter painter( &mCashe );
    SdContext context( mGrid, &painter );
    SdConverterView cv( &context, s, mOrigin, mScale.getScale() );
    cv.getMatrix();

    mPixelTransform = QTransform::fromTranslate( -s.width()/2,  -s.height()/2 );
    mPixelTransform *= QTransform::fromScale( 1 / mScale.getScale(), -1 / mScale.getScale() );
    mPixelTransform *= QTransform::fromTranslate( mOrigin.x(), mOrigin.y() );
    qDebug() << "cashe" << mOrigin << mScale.getScale();

    //Очистить фон
    painter.fillRect( 0,0, s.width(), s.height(), sdEnvir->getSysColor(scGraphBack) );

    //Рисовать сетку
    if( sdEnvir->mGridView ) {
      //Не чертить сетку менее чем в minViewGrid пикселов
      if( mScale.phys2pixel(mGrid.x()) >= sdEnvir->mMinViewGrid && mScale.phys2pixel(mGrid.y()) >= sdEnvir->mMinViewGrid ) {
        //Grid color
        painter.setPen( sdEnvir->getSysColor(scGrid) );
        SdPoint tmp( mPixelTransform.map(QPoint(0,s.height())) );
        qDebug() << "tmp" << tmp;
        tmp.setX( (tmp.x() / mGrid.x() ) * mGrid.x() );
        tmp.setY( (tmp.y() / mGrid.y() ) * mGrid.y() );
        int oldx = tmp.x();
        QPoint p = context.transform().map( tmp );
        qDebug() << "p" << p;
        painter.resetTransform();
        while( p.y() > 0 ) {
          while( p.x() < s.width() ) {
            painter.drawPoint(p);
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
    painter.setTransform( context.transform(), false );
    if( modeGet() )
      modeGet()->drawStatic( &context );
    else {
      //Рисовать содержимое
      getProjectItem()->draw( &context );
      //TODO ratNet
      //GetObject()->ForEach( DrawIterator( wc, 0, GetEnvir().showRatNet ) );
      }
    mCasheDirty = false;
    }

  QPainter painter( viewport() );
  //Восстановить статическую часть
  painter.drawImage( QPoint(), mCashe );
  if( modeGet() ) {
    //Рисовать динамическую часть
    SdContext context( mGrid, &painter );
    SdConverterView cv( &context, s, mOrigin, mScale.getScale() );
    cv.getMatrix();
    modeGet()->drawDynamic( &context );
    }
  //Рисовать курсор
  //TODO cursor
  //wc.DrawHideCursor( prevPoint );
  }




//Activate new mode
void SdWEditorGraph::modeActivate(SdMode *mode)
  {
  mCasheDirty = true;
  if( mode ) mode->activate();
  }

void SdWEditorGraph::modeRestore(SdMode *mode)
  {

  }



//Display cursor positions from mPrevPoint
void SdWEditorGraph::displayCursorPositions()
  {
  double x = mPrevPoint.x();
  double y = mPrevPoint.y();
  x *= getPPM();
  y *= getPPM();
  SdPulsar::pulsar->emitSetStatusPositions( QString::number( x, 'f', 3), QString::number( y, 'f', 3) );
  }



//Get phys coord from pixel
SdPoint SdWEditorGraph::pixel2phys(QPoint pos)
  {
  //Получить физическую координату мыши из пиксельной
  SdPoint tmp( mPixelTransform.map( pos ) );

  //Если курсор по сетке - то выровнять
  if( sdEnvir->mCursorGrid ) {
    tmp.setX( (tmp.x() / mGrid.x() ) * mGrid.x() );
    tmp.setY( (tmp.y() / mGrid.y() ) * mGrid.y() );
    }

  return tmp;
  }



//Setup mouse pos. Where pos is pixel coord
void SdWEditorGraph::updateMousePos(QMouseEvent *event)
  {
  SdPoint pos = pixel2phys( event->pos() );
  if( pos != mPrevPoint ) {
    //was a mouse mooving
    mPrevPoint = pos;
    //update actual mouse pos
    displayCursorPositions();
    }
  }



void SdWEditorGraph::onActivateEditor()
  {
  SdPulsar::pulsar->emitSetStatusLabels( QString("X:"), QString("Y:") );
  if( modeGet() )
    SdPulsar::pulsar->emitSetStatusMessage( modeGet()->getStepHelp() );
  displayCursorPositions();
  }




void SdWEditorGraph::mousePressEvent(QMouseEvent *event)
  {
  if( modeGet() ) {
    mPrevPoint = pixel2phys( event->pos() );
    if( event->button() == Qt::LeftButton ) {
      mDownPoint = mPrevPoint;
      modeGet()->enterPoint( mPrevPoint );
      }
    else if( event->button() == Qt::RightButton )
      modeGet()->cancelPoint( mPrevPoint );
    }
  }




void SdWEditorGraph::mouseReleaseEvent(QMouseEvent *event)
  {
  mPrevPoint = pixel2phys( event->pos() );
  if( event->button() == Qt::LeftButton ) {
    if( mLeftDown && modeGet() )
      modeGet()->stopDrag( mPrevPoint );
    mLeftDown = false;
    }
  //update actual mouse pos
  displayCursorPositions();
  }




void SdWEditorGraph::mouseMoveEvent(QMouseEvent *event)
  {
  SdPoint pos = pixel2phys( event->pos() );
  if( pos != mPrevPoint ) {
    //was a mouse mooving
    if( modeGet() ) {
      if( event->button() == Qt::LeftButton ) {
        if( !mLeftDown ) {
          mDownPoint = mPrevPoint;
          mLeftDown = true;
          modeGet()->beginDrag( mDownPoint );
          }
        }
      else if( mLeftDown ) {
        mLeftDown = false;
        modeGet()->stopDrag( pos );
        }

      mPrevPoint = pos;
      //update actual mouse pos
      displayCursorPositions();
      if( mLeftDown )
        modeGet()->dragPoint( mPrevPoint );
      else
        modeGet()->movePoint( mPrevPoint );
      }
    else {
      mPrevPoint = pos;
      //update actual mouse pos
      displayCursorPositions();
      }
    }
  }




void SdWEditorGraph::wheelEvent(QWheelEvent *event)
  {
  if( modeGet() )
    modeGet()->wheel( event->angleDelta() );
  }




void SdWEditorGraph::keyPressEvent(QKeyEvent *event)
  {
  if( modeGet() )
    modeGet()->keyDown( event->key(), event->text().isEmpty() ? QChar() : event->text().at(0) );
  }

void SdWEditorGraph::keyReleaseEvent(QKeyEvent *event)
  {
  if( modeGet() )
    modeGet()->keyUp( event->key(), event->text().isEmpty() ? QChar() : event->text().at(0) );
  }

