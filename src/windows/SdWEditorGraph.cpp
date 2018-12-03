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

#include "SdWEditorGraph.h"
#include "SdWView.h"
#include "SdWCommand.h"
#include "SdDEnterPosition.h"
#include "SdDPrint.h"
#include "SdDGrid.h"
#include "SdDLayers.h"
#include "objects/SdContext.h"
#include "objects/SdEnvir.h"
#include "objects/SdConverterView.h"
#include "objects/SdGraph.h"
#include "objects/SdPulsar.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdUtil.h"
#include "objects/SdSelector.h"
#include "modes/SdMode.h"
#include "modes/SdModeView.h"
#include "modes/SdModeTZoomer.h"
#include "modes/SdModeTZoomWindow.h"
#include "modes/SdModeTRuller.h"
#include "modes/SdModeCLinearArc.h"
#include "modes/SdModeCLinearLine.h"
#include "modes/SdModeCLinearRect.h"
#include "modes/SdModeCLinearRectFilled.h"
#include "modes/SdModeCLinearRegion.h"
#include "modes/SdModeCLinearRegionFilled.h"
#include "modes/SdModeCLinearCircle.h"
#include "modes/SdModeCLinearCircleFilled.h"
#include "modes/SdModeCText.h"
#include "modes/SdModeSelect.h"
#include "modes/SdModePrintWindow.h"

#include <QPainter>
#include <QPaintEvent>
#include <QImage>
#include <QMessageBox>
#include <QDebug>
#include <QScrollBar>
#include <QtPrintSupport/QPrinter>
#include <QTimer>


SdWEditorGraph::SdWEditorGraph(SdProjectItem *item, QWidget *parent) :
  SdWEditor( parent ),
  mView(nullptr),           //View port for object display [Порт для отображения объекта]
  mMode(nullptr),           //Current active mode [Режим для исполнения операций]
  mPrevMode(nullptr),       //Previous active mode for return to it [Предыдущий режим]
  mStack(nullptr),          //Temporary mode aka zoom [Временный режим]
  mSelect(nullptr),   //Режим выделения
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

  //Tracking scroll bar position
  connect( verticalScrollBar(), &QScrollBar::valueChanged, this, [this] (int value) {
    originSet( SdPoint(originGet().x(), -value) );
    });
  connect( horizontalScrollBar(), &QScrollBar::valueChanged, this, [this] (int value) {
    originSet( SdPoint(value,originGet().y()) );
    });


  //By default - view mode.
  //If no item then no mode.
  if( item ) {
    if( item->isEditEnable() ) {
      //Create select mode for this window
      mMode = mSelect = new SdModeSelect( this, item );
      modeActivate( mSelect );
      }
    else {
      mMode = new SdModeView( this, item );
      modeActivate( mMode );
      }
    QTimer::singleShot( 200, this, &SdWEditorGraph::cmViewFit );
    }

  }


//=================================================================================================
// Scale and origin
void SdWEditorGraph::scaleSet(double scale)
  {
  mScale.scaleSet( scale );
  mCasheDirty = true;
  //qDebug() << "scale=" << mScale.scaleGet();
  update();
  }

void SdWEditorGraph::scaleStep(double step)
  {
  mScale.scaleStep(step);
  mCasheDirty = true;
  //qDebug() << "scale=" << mScale.scaleGet();
  update();
  }




void SdWEditorGraph::originSet(SdPoint org)
  {
  mOrigin = org;
  mCasheDirty = true;
  update();
  }





void SdWEditorGraph::setSelectionStatus(bool status)
  {
  SdWCommand::cmEditCopy->setEnabled( status );
  SdWCommand::cmEditCut->setEnabled( status );
  SdWCommand::cmEditDelete->setEnabled( status );
  SdWCommand::cmEditProperties->setEnabled( status );
  }





//Window zoom
void SdWEditorGraph::zoomWindow(SdRect r)
  {
  //qDebug() << "zoomWindow" << r;
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





void SdWEditorGraph::contextMenu(QMenu *menu)
  {
  menu->exec( QCursor::pos() );
  }





double SdWEditorGraph::getPPM() const
  {
  if( getProjectItem() == nullptr )
    return 1.0; //Not used because item not installed
  if( getProjectItem()->getClass() & (dctPart | dctPlate) )
    return sdEnvir->mPrtPPM;
  return sdEnvir->mSchPPM;
  }






//Handle move cursor
void SdWEditorGraph::cursorMove(int dx, int dy)
  {
  //Target point in logical coords
  SdPoint pos( mPrevPoint.x() + dx * mGrid.x(), mPrevPoint.y() + dy * mGrid.y() );

  //Target point in pixels
  SdPoint pixPos( mPixelTransform.inverted().map(pos) );

  //Set new cursor pos
  QCursor::setPos( mapToGlobal(pixPos) );
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
  //Mode set.
  //Стековый режим - снести совсем
  //Прежний режим - снести (если выделения - то просто затереть, иначе - для обычного режима
  //снести совсем, а для выделения перенести в прежний
  setFocus();
  Q_ASSERT( mode != nullptr );
  if( mStack ) {
    //Стековый режим снести совсем
    delete mStack;
    mStack = nullptr;
    }
  //Если был режим выделения - просто затереть
  if( mMode && mMode == mSelect ) {
    mSelect->reset();
    mMode = mode;
    }
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
    mStack = nullptr;
    modeRestore( mMode );
    }
  else {
    //Завершение текущего режима
    if( mMode == mSelect ) {
      //Если текущим был режим выделения, то активировать прежний, если он был
      if( mPrevMode ) {
        mMode = mPrevMode;
        mPrevMode = nullptr;
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
    SdWCommand::selectMode( mode->getIndex() );
    mode->activate();
    }

  //Update edit status command
  setSelectionStatus( mMode == mSelect && mSelect->enableCopy() );
  }




void SdWEditorGraph::modeRestore(SdMode *mode)
  {
  if( mode ) {
    mode->restore();
    SdWCommand::selectMode( mode->getIndex() );
    }

  //Update edit status command
  setSelectionStatus( mMode == mSelect && mSelect->enableCopy() );
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




void SdWEditorGraph::cmViewMeasurement()
  {
  modeCall( new SdModeTRuller( this, getProjectItem() ) );
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




void SdWEditorGraph::cmModeRegion()
  {
  modeSet( new SdModeCLinearRegion( this, getProjectItem() )  );
  }




void SdWEditorGraph::cmModeFilledRegion()
  {
  modeSet( new SdModeCLinearRegionFilled( this, getProjectItem() )  );
  }




void SdWEditorGraph::cmModeCircle()
  {
  modeSet( new SdModeCLinearCircle( this, getProjectItem() )  );
  }





void SdWEditorGraph::cmModeFilledCircle()
  {
  modeSet( new SdModeCLinearCircleFilled( this, getProjectItem() )  );
  }




void SdWEditorGraph::cmModeArc()
  {
  modeSet( new SdModeCLinearArc( this, getProjectItem() )  );
  }




void SdWEditorGraph::cmModeText()
  {
  modeSet( new SdModeCText( this, getProjectItem() ) );
  }




void SdWEditorGraph::cmModeSelect()
  {
  modeSet( mSelect );
  }



void SdWEditorGraph::cmViewFit()
  {
  //Collect fit rect
  SdRect fit;
  getProjectItem()->forEach( dctAll, [&fit](SdObject *obj) {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph && graph->isVisible() ) {
      //qDebug() << graph->getOverRect();
      if( fit.isEmpty() )
        fit = graph->getOverRect();
      else
        fit.grow( graph->getOverRect() );
      }
    return true;
    } );

  //If fit rect not empty - expand this rect on all view
  if( !fit.isEmpty() )
    zoomWindow( fit );
  }





void SdWEditorGraph::cmViewGrid()
  {
  SdDGrid gridDlg( SdUtil::log2phys( mGrid, getPPM() ), this );
  if( gridDlg.exec() ) {
    mGrid = SdUtil::phys2log( gridDlg.getGrid(), getPPM() );
    dirtyCashe();
    update();
    }
  }





void SdWEditorGraph::cmViewLayers()
  {
  dirtyCashe();
  update();
  }




void SdWEditorGraph::cmEnterPosition()
  {
  //Handle enter cursor position
  SdDEnterPosition dep(this);
  while( dep.exec() ) {
    //Build point from entered coord
    SdPoint p;
    p.setX( SdUtil::phys2log( dep.getX(), getPPM() ) );
    p.setY( SdUtil::phys2log( dep.getY(), getPPM() ) );
    //qDebug() << "enter" << p;
    if( dep.getRef() )
      //Referenced coord
      p.move( mPrevEnter );
    //qDebug() << "enter" << p;
    //Absolute coord
    mPrevEnter = p;
    if( modeGet() )
      modeGet()->enterPoint( p );
    }
  }





void SdWEditorGraph::printDialog(SdRect wnd)
  {
  int dlgRes;
  QPrinter printer;
  //By default print whole contens
  SdRect over = getProjectItem()->getOverRect();
  SdRect sel;
  if( mSelect ) sel = mSelect->getFragmentOver();
  SdDPrint printDlg( over, sel, wnd, getPPM(), &printer, this );
  dlgRes = printDlg.exec();
  if( dlgRes == 1 ) {
    //Print
    int printArea = printDlg.getPrintArea();
    double scale = printDlg.getScaleFactor();
    bool colorPrint = printDlg.isColor();
    int zeroWidth = printDlg.getZeroWidth();
    if( printArea == SDPA_FULL_OBJECT )
      print( printer, printDlg.getWindow(), zeroWidth, colorPrint, scale, nullptr );
    else if( printArea == SDPA_SELECTION )
      print( printer, printDlg.getWindow(), zeroWidth, colorPrint, scale, mSelect->getFragment() );
    else
      print( printer, printDlg.getWindow(), zeroWidth, colorPrint, scale, nullptr );
    }
  else if( dlgRes == 2 ) {
    //Select window
    modeCall( new SdModePrintWindow( this, getProjectItem() )  );
    }
  }




//Print projectItem or selection in desired window
void SdWEditorGraph::print(QPrinter &printer, SdRect wnd, int zeroWidth, bool colorPrint, double scale, SdSelector *selector)
  {
  //Size of page in pixels
  QRect r = printer.pageRect(QPrinter::DevicePixel).toRect();

  //Create painter, context and converter
  QPainter painter( &printer );
  SdContext context( mGrid, &painter );
  SdConverterView cv( QSize(r.width(),r.height()), wnd.center(), scale );
  context.setConverter( &cv );

  if( !colorPrint )
    context.setOverColor( Qt::black );

  if( zeroWidth )
    context.setOverZeroWidth( zeroWidth );

  //Print
  if( selector == nullptr )
    getProjectItem()->draw( &context );
  else
    selector->draw( &context );
  }









//=================================================================================================
// Paint process
//Parametrized paint process
void SdWEditorGraph::paintProcess(bool viewer)
  {
  if( viewport() == nullptr ) return;
  QSize s = viewport()->size();
  mClientSize.set( s.width(), s.height() );

  if( getProjectItem() == nullptr ) {
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
    //Clear backspace
    mCashe.fill( sdEnvir->getSysColor(scGraphBack) );

    QPainter painter( &mCashe );
    SdContext context( mGrid, &painter );
    SdConverterView cv( s, mOrigin, mScale.scaleGet() );
    context.setConverter( &cv );

    //Matrix transformation from screen coord to phis coord
    mPixelTransform = QTransform::fromTranslate( -s.width()/2,  -s.height()/2 );
    mPixelTransform *= QTransform::fromScale( 1 / mScale.scaleGet(), -1 / mScale.scaleGet() );
    mPixelTransform *= QTransform::fromTranslate( mOrigin.x(), mOrigin.y() );
    //qDebug() << "cashe" << mOrigin << mScale.scaleGet();

    //Draw grid
    if( !viewer && sdEnvir->mGridShow && mGrid.x() > 0 ) {
      //We don't draw grid with cell less then minViewGrid pixels
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
    //Draw contens
    //painter.setTransform( context.transform(), false );
    if( modeGet() )
      //Thrue mode
      modeGet()->drawStatic( &context );
    else {
      //Draw contens item
      getProjectItem()->draw( &context );
      }

    //Get over rect
    SdRect over = getProjectItem()->getOverRect();
    //Calculate page size
    double pagex = qMax( s.width(), 1);
    pagex /= mScale.scaleGet();
    double pagey = qMax( s.height(), 1 );
    pagey /= mScale.scaleGet();

    if( over.isEmpty() )
      //If over is empty create over rect with default size
      over.set( SdPoint(mOrigin.x() - static_cast<int>(pagex / 2.0), mOrigin.y() - static_cast<int>(pagey / 2.0)),
                SdPoint(mOrigin.y() + static_cast<int>(pagex / 2.0), mOrigin.y() + static_cast<int>(pagey / 2.0)) );

    if( over != mLastOver ) {
      //Over rect changed - recalc scroll bars
      if( over.width() < pagex ) {
        SdPoint center = over.center();
        over.setWidth( static_cast<int>(pagex) );
        over.moveCenter( center );
        }
      if( over.height() < pagey ) {
        SdPoint center = over.center();
        over.setHeight( static_cast<int>(pagey) );
        over.moveCenter( center );
        }
      //Update scrollbars
      //Range of scrollbars is twice of over rect existing picture
      SdPoint center = over.center();
      int ow = over.width();
      int oh = over.height();
      over.setWidth( ow * 2 );
      over.setHeight( oh * 2 );
      over.moveCenter( center );

      verticalScrollBar()->setRange( -over.getTop(), -over.getBottom() );
      verticalScrollBar()->setSingleStep( mGrid.y() );
      verticalScrollBar()->setPageStep( static_cast<int>(pagey) );
      verticalScrollBar()->setValue( -mOrigin.y() );

      horizontalScrollBar()->setRange( over.getLeft(), over.getRight() );
      horizontalScrollBar()->setSingleStep( mGrid.x() );
      horizontalScrollBar()->setPageStep( static_cast<int>(pagex) );
      horizontalScrollBar()->setValue( mOrigin.x() );

      mLastOver = over;
      }


    mCasheDirty = false;
    }

  QPainter painter( viewport() );
  //Восстановить статическую часть
  painter.drawImage( QPoint(), mCashe );

  SdContext context( mGrid, &painter );
  SdConverterView cv( s, mOrigin, mScale.scaleGet() );
  context.setConverter( &cv );

  if( modeGet() ) {
    //Рисовать динамическую часть
    modeGet()->drawDynamic( &context );
    }

  //On plate object draw rat net and rule errors
  if( !viewer && getProjectItem()->getClass() == dctPlate ) {
    SdPItemPlate *plate = dynamic_cast<SdPItemPlate*>( getProjectItem() );
    Q_ASSERT( plate != nullptr );
    if( sdEnvir->mShowRatNet )
      plate->drawRatNet( &context );
    if( sdEnvir->mShowRuleErrors )
      plate->drawRuleErrors( &context );
    }

  //Crosshair cursor paint [Рисовать курсор]
  if( !viewer )
    context.drawCursor( mPrevPoint );
  }

//=================================================================================================
// Event handlers

//Paint event
void SdWEditorGraph::paintEvent(QPaintEvent *event)
  {
  event->accept();
  paintProcess( false );
  }




void SdWEditorGraph::mousePressEvent(QMouseEvent *event)
  {
  if( modeGet() ) {
    mPrevPoint = pixel2phys( event->pos() );
    mPrevEnter = mPrevPoint;
    if( event->button() == Qt::LeftButton ) {
      mDownPoint = mPrevPoint;
      modeGet()->enterPoint( mPrevPoint );
      }
    else if( event->button() == Qt::MiddleButton )
      mPrevEnter = modeGet()->enterPrev();
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
      if( event->buttons() & Qt::LeftButton ) {
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
    if( sdEnvir->mCursorView != dcvNone )
      update();
    }
  }




void SdWEditorGraph::wheelEvent(QWheelEvent *event)
  {
  //At first try handle event with mode
  //if mode not handle event, then make defaul behavior
  if( modeGet() && modeGet()->wheel( event->angleDelta() ) )
    return;
  int delta = event->angleDelta().y() / 120;
  //qDebug() << "wheel" << event->angleDelta().y();
  if( event->modifiers() & Qt::ControlModifier ) {
    //Move by horizontal
    originSet( SdPoint(originGet().x() + delta * gridGet().x(), originGet().y()) );
    }
  else if( event->modifiers() & Qt::ShiftModifier ) {
    //Scale
    if( event->angleDelta().y() < 0 ) scaleStep(1.2);
    else scaleStep(0.83);
    }
  else {
    //Move by vertical
    originSet( SdPoint(originGet().x(), originGet().y() + delta * gridGet().y()) );
    }
  }




void SdWEditorGraph::keyPressEvent(QKeyEvent *event)
  {
  //Help key
  if( event->key() == Qt::Key_F1 ) {
    if( modeGet() )
      SdPulsar::sdPulsar->emitHelpTopic( modeGet()->getStepThema() );
    }
  else if( modeGet() )
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
  SdPulsar::sdPulsar->emitSetStatusPositions( QString::number( x, 'f', 3), QString::number( y, 'f', 3) );
  }



//Get phys coord from pixel
SdPoint SdWEditorGraph::pixel2phys(QPoint pos)
  {
  //Получить физическую координату мыши из пиксельной
  SdPoint tmp( mPixelTransform.map( pos ) );

  //Если курсор по сетке - то выровнять
  if( sdEnvir->mCursorAlignGrid ) {
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
  SdPulsar::sdPulsar->emitSetStatusLabels( QString("X:"), QString("Y:") );
  if( modeGet() ) {
    modeGet()->restore();
    SdWCommand::selectMode( modeGet()->getIndex() );
    }
  displayCursorPositions();
  //Update edit status command
  setSelectionStatus( mMode == mSelect && mSelect->enableCopy() );
  //Change status of clipboard
  SdWCommand::cmEditPaste->setEnabled( SdSelector::isClipboardAvailable() );
  mCasheDirty = true;
  }





void SdWEditorGraph::cmFilePrint()
  {
  //Call printDlg with default window rect
  printDialog( SdRect() );
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




void SdWEditorGraph::cmEditCut()
  {
  //Only for selecting mode
  if( mMode == mSelect && mSelect != nullptr )
    mSelect->cut();
  }




void SdWEditorGraph::cmEditCopy()
  {
  //Only for selecting mode
  if( mMode == mSelect && mSelect != nullptr )
    mSelect->copy();
  }




void SdWEditorGraph::cmEditPaste()
  {
  //When paste set Select mode to current if it is not
  if( mMode != mSelect )
    modeSet( mSelect );
  //Only for selecting mode
  if( mMode == mSelect && mSelect != nullptr )
    mSelect->paste();
  }




void SdWEditorGraph::cmEditDelete()
  {
  //Only for selecting mode
  if( mMode == mSelect && mSelect != nullptr )
    mSelect->deleteSelected();
  }





void SdWEditorGraph::cmEditSelectAll()
  {
  //When selecting all set Select mode to current if it is not
  if( mMode != mSelect )
    modeSet( mSelect );
  //Only for selecting mode
  if( mMode == mSelect && mSelect != nullptr )
    mSelect->selectAll();
  }




void SdWEditorGraph::cmEditUnSelect()
  {
  //Only for selecting mode
  if( mMode == mSelect && mSelect != nullptr )
    mSelect->unselect(false);
  }






void SdWEditorGraph::cmClipboardChange()
  {
  //Change status of clipboard
  SdWCommand::cmEditPaste->setEnabled( SdSelector::isClipboardAvailable() );
  }



void SdWEditorGraph::resizeEvent(QResizeEvent *event)
  {
  //Invalidate cache
  dirtyCashe();
  SdWEditor::resizeEvent( event );
  }

