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
#include "SdWCommand.h"
#include "objects/SdContext.h"
#include "objects/SdEnvir.h"
#include "objects/SdConverterView.h"
#include "objects/SdGraph.h"
#include "objects/SdPulsar.h"
#include "modes/SdMode.h"
#include "modes/SdModeView.h"
#include "modes/SdModeTZoomer.h"
#include "modes/SdModeTZoomWindow.h"
#include "modes/SdModeCLinearLine.h"
#include "modes/SdModeCLinearRect.h"
#include "modes/SdModeCLinearRectFilled.h"
#include "modes/SdModeCText.h"

#include <QPainter>
#include <QPaintEvent>
#include <QImage>
#include <QMessageBox>
#include <QDebug>

SdWEditorGraph::SdWEditorGraph(SdProjectItem *item, QWidget *parent) :
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

  setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );

  //By default - view mode.
  //If no item then no mode.
  if( item ) {
    mMode = new SdModeView( this, item );
    modeActivate( mMode );
    }
  }


//=================================================================================================
// Scale and origin
void SdWEditorGraph::scaleSet(double scale)
  {
  mScale.scaleSet( scale );
  mCasheDirty = true;
  qDebug() << "scale=" << mScale.scaleGet();
  update();
  }

void SdWEditorGraph::scaleStep(double step)
  {
  mScale.scaleStep(step);
  mCasheDirty = true;
  qDebug() << "scale=" << mScale.scaleGet();
  update();
  }




void SdWEditorGraph::originSet(SdPoint org)
  {
  mOrigin = org;
  mCasheDirty = true;
  update();
  }





//Window zoom
void SdWEditorGraph::zoomWindow(SdRect r)
  {
  originSet( r.center() );
  //Вычислить новый масштаб
  double scale = mScale.scaleGet();
  if( r.width() )
    scale = static_cast<double>(mClientSize.x()) / static_cast<double>(r.width());
  double scaleY = mScale.scaleGet();
  if( r.height() )
    scaleY = static_cast<double>(mClientSize.y()) / static_cast<double>(r.height());
  if( scale > scaleY ) scale = scaleY;
  scaleSet( scale );
  }







//=================================================================================================
// Modes management

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
  //Check if author
  if( getProjectItem()->isAnotherAuthor() ) {
    int r = QMessageBox::question( this, tr("Warning"), tr("Object was created by another author '%1'. Change to your name?").arg(getProjectItem()->getAuthor()) );
    if( r == QMessageBox::No ) {
      //Alien author object edit not allowed
      if( mode ) delete mode;
      return;
      }
    //TODO Check if name unical for this author

    getProjectItem()->updateAuthor();
    getProjectItem()->updateCreationTime();
    }
  //Mode set. Its also set edit status for graph object
  //TODO set edit status
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




//Activate new mode
void SdWEditorGraph::modeActivate(SdMode *mode)
  {
  mCasheDirty = true;
  if( mode ) {
    mode->activate();
    SdWCommand::selectMode( mode->getIndex() );
    }
  }

void SdWEditorGraph::modeRestore(SdMode *mode)
  {
  if( mode ) {
    mode->restore();
    SdWCommand::selectMode( mode->getIndex() );
    }
  }




//=================================================================================================
// Modes

void SdWEditorGraph::cmViewZoomIn()
  {
  modeCall( new SdModeTZoomer(true, this, getProjectItem() ) );
  }




void SdWEditorGraph::cmViewZoomOut()
  {
  modeCall( new SdModeTZoomer(false, this, getProjectItem() ) );
  }




void SdWEditorGraph::cmViewWindow()
  {
  modeCall( new SdModeTZoomWindow( this, getProjectItem() ) );
  }



void SdWEditorGraph::cmPropChanged()
  {
  if( modeGet() )
    modeGet()->propGetFromBar();
  }




void SdWEditorGraph::cmModeLine()
  {
  modeSet( new SdModeCLinearLine( this, getProjectItem() ) );
  }




void SdWEditorGraph::cmModeRect()
  {
  modeSet( new SdModeCLinearRect( this, getProjectItem() ) );
  }




void SdWEditorGraph::cmModeFilledRect()
  {
  modeSet( new SdModeCLinearRectFilled( this, getProjectItem() ) );
  }




void SdWEditorGraph::cmModeText()
  {
  modeSet( new SdModeCText( this, getProjectItem() ) );
  }



void SdWEditorGraph::cmViewFit()
  {
  //Collect fit rect
  SdRect fit;
  getProjectItem()->forEach( dctAll, [&fit](SdObject *obj) {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph && graph->isVisible() )
      fit.grow( graph->getOverRect() );
    return true;
    } );

  //If fit rect not empty - expand this rect on all view
  if( !fit.isEmpty() )
    zoomWindow( fit );
  }




//=================================================================================================
// Event handlers

//Paint event
void SdWEditorGraph::paintEvent(QPaintEvent *event)
  {
  event->accept();
  if( viewport() == nullptr ) return;
  QSize s = viewport()->size();
  mClientSize.set( s.width(), s.height() );

  if( getProjectItem() == 0 ) {
    //Object not present. Show message
    QPainter painter( viewport() );
    QRect r(0,0, s.width(), s.height());
    painter.fillRect( r, sdEnvir->getSysColor(scGraphBack) );
    painter.drawText( r, Qt::AlignCenter,  tr("No object") );
    return;
    }

  if( mCasheDirty ) {
    //Redraw static part
    mCashe = QImage( s, QImage::Format_ARGB32_Premultiplied );
    //Очистить фон
    mCashe.fill( sdEnvir->getSysColor(scGraphBack) );

    QPainter painter( &mCashe );
    SdContext context( mGrid, &painter );
    SdConverterView cv( s, mOrigin, mScale.scaleGet() );
    context.setConverter( &cv );

    mPixelTransform = QTransform::fromTranslate( -s.width()/2,  -s.height()/2 );
    mPixelTransform *= QTransform::fromScale( 1 / mScale.scaleGet(), -1 / mScale.scaleGet() );
    mPixelTransform *= QTransform::fromTranslate( mOrigin.x(), mOrigin.y() );
    //qDebug() << "cashe" << mOrigin << mScale.scaleGet();

    //Рисовать сетку
    if( sdEnvir->mGridView ) {
      //Не чертить сетку менее чем в minViewGrid пикселов
      if( mScale.phys2pixel(mGrid.x()) >= sdEnvir->mMinViewGrid && mScale.phys2pixel(mGrid.y()) >= sdEnvir->mMinViewGrid ) {
        //Grid color
        painter.setPen( sdEnvir->getSysColor(scGrid) );
        SdPoint tmp( mPixelTransform.map(QPoint(0,s.height())) );
        //qDebug() << "tmp" << tmp;
        tmp.setX( (tmp.x() / mGrid.x() ) * mGrid.x() );
        tmp.setY( (tmp.y() / mGrid.y() ) * mGrid.y() );
        int oldx = tmp.x();
        QPoint p = context.transform().map( tmp );
        //qDebug() << "p" << p;
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
    SdConverterView cv( s, mOrigin, mScale.scaleGet() );
    context.setConverter( &cv );
    modeGet()->drawDynamic( &context );
    }
  //Рисовать курсор
  //TODO cursor
  //wc.DrawHideCursor( prevPoint );
  }




void SdWEditorGraph::mousePressEvent(QMouseEvent *event)
  {
  if( modeGet() ) {
    mPrevPoint = pixel2phys( event->pos() );
    if( event->button() == Qt::LeftButton ) {
      mDownPoint = mPrevPoint;
      modeGet()->enterPoint( mPrevPoint );
      }
    else if( event->button() == Qt::MiddleButton )
      modeGet()->enterPrev();
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




//=================================================================================================
// Misc

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
    if( tmp.x() < 0 )
      tmp.setX( ((tmp.x() - mGrid.x()/2) / mGrid.x() ) * mGrid.x() );
    else
      tmp.setX( ((tmp.x() + mGrid.x()/2) / mGrid.x() ) * mGrid.x() );

    if( tmp.y() < 0 )
      tmp.setY( ((tmp.y() - mGrid.y()/2) / mGrid.y() ) * mGrid.y() );
    else
      tmp.setY( ((tmp.y() + mGrid.y()/2) / mGrid.y() ) * mGrid.y() );
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
  SdWEditor::onActivateEditor();
  SdPulsar::pulsar->emitSetStatusLabels( QString("X:"), QString("Y:") );
  if( modeGet() ) {
    modeGet()->restore();
    SdWCommand::selectMode( modeGet()->getIndex() );
    }
  displayCursorPositions();
  }




void SdWEditorGraph::cmEditUndo()
  {
  if( modeGet() )
    modeGet()->reset();
  mCasheDirty = true;
  update();
  }




void SdWEditorGraph::cmEditRedo()
  {
  if( modeGet() )
    modeGet()->reset();
  mCasheDirty = true;
  update();
  }

