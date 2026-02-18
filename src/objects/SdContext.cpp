/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Context for graphics draw
*/

#include "SdContext.h"
#include "SdConverter.h"
#include "SdEnvir.h"
#include "SdConverterText.h"
#include <QPainter>
#include <QFont>
#include <QDebug>


SdContext::SdContext(SdPoint grid, QPainter *painter) :
  mPainter(painter),
  mConverter(nullptr),
  mGrid(grid),
  mSelector(nullptr),
  mTransform(),
  mScaler(1.0),
  mParamMap(nullptr),
  mAngle(0),
  mMirror(false),
  mPairLayer(false),
  mOverOn(false),      //True if overriding is on
  mZeroOn(false)       //True if overriding zero width line on
  {
  mShowFields = SdEnvir::instance()->mShowFields;
  }




SdContext *SdContext::setConverter(SdConverter *c)
  {
  //Append converter to stack
  c->mContext = this;
  c->mNext = mConverter;
  mConverter = c;

  updateConverter();
  return this;
  }




void SdContext::removeConverter(SdConverter *c)
  {
  if( c == mConverter ) {
    mConverter = mConverter->mNext;
    }
  updateConverter();
  }




void SdContext::setSelector(SdSelector *selector)
  {
  mSelector = selector;
  }




void SdContext::line(SdPoint a, SdPoint b)
  {
  mPainter->drawLine( mTransform.map(a), mTransform.map(b) );
  }




void SdContext::line(SdPoint a, SdPoint b, const SdPropLine &prop)
  {
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    setProp( prop );
    line( a, b );
    }
  }




void SdContext::symPin(SdPoint a, SdLayer *layer)
  {
  if( mSelector || (layer != nullptr && layer->isVisible()) ) {
    cross( a, SdEnvir::instance()->mSymPinSize, convertColor( layer ) );
    }
  }




void SdContext::partPin(SdPoint a, SdLayer *layer)
  {
  if( mSelector || (layer != nullptr && layer->isVisible()) ) {
    setPen( 0, convertColor( layer ), dltSolid );
    circle( a, SdEnvir::instance()->mPartPinSize );
    }
  }





void SdContext::cross(SdPoint a, int size, QColor color)
  {
  setPen( 0, color, dltSolid );
  line( SdPoint(a.x()-size,a.y()-size), SdPoint(a.x()+size,a.y()+size ) );
  line( SdPoint(a.x()+size,a.y()-size), SdPoint(a.x()-size,a.y()+size ) );
  }




void SdContext::drawLineArrow(SdPoint p1, SdPoint p2, QColor color, int arrowSize)
  {
  mPainter->setOpacity( 1.0 );
  mPainter->setPen( color );
  line(p1,p2);
  SdPoint mp1 = mTransform.map(p1);
  SdPoint mp2 = mTransform.map(p2);
  //On point2 draw arrow with arrow size
  SdPvAngle angle = mp1.getAngle( mp2 );
  arrowSize = mScaler.phys2pixel(arrowSize);
  SdPoint e1( mp2.x() + arrowSize, mp2.y() - arrowSize / 2 );
  e1.rotate( mp2, angle );
  SdPoint e2( mp2.x() + arrowSize, mp2.y() + arrowSize / 2 );
  e2.rotate( mp2, angle );
  mPainter->drawLine( e1.x(), e1.y(), mp2.x(), mp2.y() );
  mPainter->drawLine( e2.x(), e2.y(), mp2.x(), mp2.y() );
  }




void SdContext::quadrangle(SdQuadrangle q, const SdPropLine &prop)
  {
  //Draw 4 edges
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    setPen( prop.mWidth, prop.mLayer.layer(), prop.mType );
    line( q.p1, q.p2 );
    line( q.p2, q.p3 );
    line( q.p3, q.p4 );
    line( q.p4, q.p1 );
    }
  }




void SdContext::rect(SdRect r)
  {
  line( r.getTopLeft(), r.getTopRight() );
  line( r.getTopRight(), r.getBottomRight() );
  line( r.getBottomRight(), r.getBottomLeft() );
  line( r.getBottomLeft(), r.getTopLeft() );
  }



void SdContext::rect(SdRect r, const SdPropLine &prop)
  {
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    setProp( prop );
    rect( r );
    }
  }




void SdContext::fillRect(SdRect r)
  {
  mPainter->drawPolygon( mTransform.mapToPolygon(r) );
  }



void SdContext::fillRect(SdRect r, QColor color)
  {
  if( mPainter->isActive() )
    mPainter->setPen( color );
  setBrush( color );
  fillRect( r );
  }




void SdContext::fillRect(SdRect r, SdLayerPtr layer)
  {
  if( layer != nullptr && ( mSelector || convertLayer(layer)->isVisible() ) )
    fillRect( r, convertColor(layer) );
  }




void SdContext::arc(SdPoint center, SdPoint start, SdPoint stop)
  {
  double radius = center.getDistance(start);
  QRectF r( center.x()-radius, center.y()-radius, 2.0*radius, 2.0*radius );
  double ang = mAngle;
  ang /= 1000.0;
  double startAngle = start.getAngleDegree( center );
  double arcAngle = stop.getAngleDegree( center ) - startAngle;
  startAngle += ang;
  if( arcAngle < 0 )
    arcAngle += 360.0;
  if( mMirror )
    mPainter->drawArc( mTransform.mapRect(r), static_cast<int>(startAngle * 16.0), static_cast<int>(-arcAngle * 16.0) );
  else
    mPainter->drawArc( mTransform.mapRect(r), static_cast<int>(startAngle * 16.0), static_cast<int>(arcAngle * 16.0) );
  }




void SdContext::arc(SdPoint center, SdPoint start, SdPoint stop, const SdPropLine &prop)
  {
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    setProp( prop );
    arc( center, start, stop );
    }
  }




void SdContext::circle(SdPoint center, int radius)
  {
  mPainter->setBrush( QBrush(Qt::transparent) );
  SdRect r( SdPoint(center.x()-radius,center.y()-radius), SdPoint(center.x()+radius,center.y()+radius) );
  mPainter->drawEllipse( mTransform.mapRect(r) );
  }



void SdContext::circle(SdPoint center, int radius, const SdPropLine &prop)
  {
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    setProp( prop );
    circle( center, radius );
    }
  }




void SdContext::circleFill(SdPoint center, int radius)
  {
  SdRect r( SdPoint(center.x()-radius,center.y()-radius), SdPoint(center.x()+radius,center.y()+radius) );
  mPainter->drawEllipse( mTransform.mapRect(r) );
  }




void SdContext::circleFill(SdPoint center, int radius, QColor color)
  {
  if( mPainter->isActive() )
    mPainter->setPen( color );
  setBrush( color );
  circleFill( center, radius );
  }






void SdContext::circleFill(SdPoint center, int radius, SdLayerPtr layer)
  {
  if( layer != nullptr && ( mSelector || convertLayer(layer)->isVisible() ) )
    circleFill( center, radius, convertColor(layer) );
  }



void SdContext::polygonFill(const QPolygonF &poly)
  {
  mPainter->drawPolygon( mTransform.map(poly) );
  }



void SdContext::polygonFill(const QPolygonF &poly, QColor color)
  {
  if( mPainter->isActive() )
    mPainter->setPen( color );
  setBrush( color );
  polygonFill( poly );
  }




void SdContext::textEx(SdPoint pos, SdRect &over, const QString str, int dir, int horz, int vert, int cursor, SdPoint *cp1, SdPoint *cp2, SdRect *sel, int start, int stop  )
  {
  //Get over rect of text
  if( str.isEmpty() )
    over.set( mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, QString("H") ) );
  else {
    over.set( mPainter->boundingRect( 0,0, 0,0, Qt::AlignLeft | Qt::AlignTop, str ) );

    //Align text with flags
    switch( horz ) {
      case dhjLeft   : break;
      case dhjCenter : over.moveLeft( -over.width() / 2 ); break;
      case dhjRight  : over.moveRight( 0 ); break;
      }
    }

  switch( vert ) {
    case dvjTop : break;
    case dvjMiddle : over.moveTop( -over.height() / 2 ); break;
    case dvjBottom : over.moveBottom( 0 ); break;
    }

  SdConverterText cnv( mTransform.map(pos), dir + mAngle, mMirror );
  QTransform txt = cnv.getMatrix();
  mPainter->setTransform( txt, false );

  if( !str.isEmpty() )
    mPainter->drawText( over, Qt::AlignLeft | Qt::AlignTop, str );
  mPainter->resetTransform();

  if( cp1 && cp2 ) {
    if( cursor == 0 ) {
      *cp1 = txt.map( over.getTopLeft() );
      *cp2 = txt.map( over.getBottomLeft() );
      }
    else {
      QRect rpos = mPainter->boundingRect( over, Qt::AlignLeft | Qt::AlignTop, str.left(cursor) );
      *cp1 = txt.map( rpos.topRight() );
      *cp2 = txt.map( rpos.bottomRight() );
      }
    mPainter->drawLine( *cp1, *cp2 );
    }

  if( sel ) {
    //Fill selection rect
    QRect rStart = mPainter->boundingRect( over, Qt::AlignLeft | Qt::AlignTop, str.left(start) );
    QRect rStop  = mPainter->boundingRect( over, Qt::AlignLeft | Qt::AlignTop, str.left(stop) );
    QRect res;
    res.setTopLeft( rStart.topRight() );
    res.setBottomRight( rStop.bottomRight() );
    sel->set( mTransform.inverted().mapRect( txt.mapRect( res ) ) );
    }

  over.set( mTransform.inverted().mapRect( txt.mapRect( over ) ) );
  }




void SdContext::text( SdPoint pos, SdRect &over, const QString str, const SdPropText &prop)
  {
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    setFont( prop );
    textEx( pos, over, str, prop.mDir, prop.mHorz, prop.mVert );
    }
  }







void SdContext::region(const SdPointList &points, bool autoClose)
  {
  for( int i = 1; i < points.count(); i++ )
    line( points.at(i-1), points.at(i) );
  if( autoClose && points.first() != points.last() )
    line( points.first(), points.last() );
  }



void SdContext::region( const SdPointList &points, const SdPropLine &prop, bool autoClose )
  {
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    if( points.count() > 1 ) {
      setPen( prop.mWidth, prop.mLayer.layer(), prop.mType );
      region( points, autoClose );
      }
    }
  }






void SdContext::regionFill(const SdPointList &points, const SdPropLine &prop)
  {
  if( (mSelector || prop.mLayer.layer(mPairLayer)->isVisible()) && (points.count() > 2) ) {
    QColor color = convertColor(prop.mLayer.layer());
    mPainter->setPen( color );
    setBrush( color );
    mPainter->drawPolygon( mTransform.map( points ) );
    }
  }





void SdContext::polygon(const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer)
  {
  //To draw polygon with windows we need intermediate picture image
  //1. fill this image with transparent color
  //2. we draw polygon on image
  //3. we draw windows with transparent fill color
  //4. image copy to painter
  QImage intermediate( mPainter->device()->width(), mPainter->device()->height(), QImage::Format_ARGB32 );

  //1. fill this image with transparent color
  intermediate.fill( Qt::transparent );
  QPainter image(&intermediate);

  //2. we draw polygon on image
  image.setPen( convertColor(layer) );
  image.setBrush( convertColor(layer) );
  image.drawPolygon( mTransform.map( points ) );

  //3. we draw windows with transparent fill color
  image.setCompositionMode( QPainter::CompositionMode_Source );
  image.setPen( Qt::transparent );
  image.setBrush( Qt::transparent );
  //image.setBrush( Qt::yellow );
  for( const SdPolyWindow &win : windows ) {
    if( win.getRadius() >= 0 ) {
      //Circle window
      SdPoint center = win.getP1();
      int radius = win.getRadius();
      SdRect r( SdPoint(center.x()-radius,center.y()-radius), SdPoint(center.x()+radius,center.y()+radius) );
      image.drawEllipse( mTransform.mapRect(r) );
      }
    else {
      //Rectangle window
      image.drawPolygon( mTransform.map( win.polygon() ) );
      }
    }

  //4. image copy to painter
  //Here we store previous opacity
  double op = mPainter->opacity();
  mPainter->setOpacity( SdEnvir::instance()->mPolygonOpacity * op );
  mPainter->drawImage( 0, 0, intermediate );
  //Restore previous opacity
  mPainter->setOpacity( op );
  }




void SdContext::smartPoint(SdPoint a, SdSnapMask smartMask)
  {
  if( smartMask ) {
    //Рисовать точку
    QPoint p = mTransform.map(a);
    //Точка попадает в область видимости - рисуем
    mPainter->resetTransform();
    if( p.x() >= 0 && p.x() <= mPainter->device()->width() &&
        p.y() >= 0 && p.y() <= mPainter->device()->height() ) {

      //Подготовить перо
      mPainter->setPen( QPen(QBrush(SdEnvir::instance()->getSysColor(scSmart)), SdEnvir::instance()->mSmartWidth) );
      mPainter->setBrush( Qt::transparent );
      int sm = SdEnvir::instance()->mSmartSize;

      //Приступить к рисованию
      if( smartMask & (snapNearestNet | snapNearestNetEnd | snapNearestPin | snapNextPin | snapNetPoint) ) {
        //Ромбик
        QPoint p1( p.x() - sm, p.y() );
        QPoint p2( p.x(), p.y() - sm );
        QPoint p3( p.x() + sm, p.y() );
        QPoint p4( p.x(), p.y() + sm );
        mPainter->drawLine( p1, p2 );
        mPainter->drawLine( p2, p3 );
        mPainter->drawLine( p3, p4 );
        mPainter->drawLine( p4, p1 );
        }
      else if( smartMask & snapParallel ) {
        //Две параллельных черточки
        int dm = sm / 2 + 1;
        QPoint p1( p.x() - dm, p.y() - sm );
        QPoint p2( p.x() - dm, p.y() + sm );
        QPoint p3( p.x() + dm, p.y() - sm );
        QPoint p4( p.x() + dm, p.y() + sm );
        mPainter->drawLine( p1, p2 );
        mPainter->drawLine( p3, p4 );
        }
      else if( smartMask & snapPerpendecular ) {
        //Две перпендикулярных линии
        QPoint p1( p.x() - sm, p.y() - sm );
        QPoint p2( p.x() + sm, p.y() - sm );
        QPoint p3( p.x(), p.y() - sm );
        QPoint p4( p.x(), p.y() + sm );
        mPainter->drawLine( p1, p2 );
        mPainter->drawLine( p3, p4 );
        }
      else if( smartMask & snapTangent ) {
        //Кружок и касательная
        QPoint p1( p.x() - sm, p.y() - sm );
        QPoint p2( p.x() + sm, p.y() - sm );
        mPainter->drawEllipse( p, sm, sm );
        mPainter->drawLine( p1, p2 );
        }
      else if( smartMask & snapQuadrant ) {
        //Кружок и четыре точки по квадрантам
        mPainter->drawEllipse( p, sm, sm );
        mPainter->drawEllipse( QPoint(p.x() + sm, p.y()), 1, 1 );
        mPainter->drawEllipse( QPoint(p.x() - sm, p.y()), 1, 1 );
        mPainter->drawEllipse( QPoint(p.x(), p.y() + sm), 1, 1 );
        mPainter->drawEllipse( QPoint(p.x(), p.y() - sm), 1, 1 );
        }
      else if( smartMask & snapCenter ) {
        //Кружок
        mPainter->drawEllipse( p, sm, sm );
        }
      else if( smartMask & snapViaPoint ) {
        //Two displaced circles
        mPainter->drawEllipse( p, sm, sm );
        p += QPoint( sm/2, sm/2 );
        mPainter->drawEllipse( p, sm, sm );
        }
      else if( smartMask & (snapExtension | snapEndPoint) ) {
        //Квадратик
        QPoint p1( p.x() - sm, p.y() - sm );
        QPoint p2( p.x() + sm, p.y() - sm );
        QPoint p3( p.x() + sm, p.y() + sm );
        QPoint p4( p.x() - sm, p.y() + sm );
        mPainter->drawLine( p1, p2 );
        mPainter->drawLine( p2, p3 );
        mPainter->drawLine( p3, p4 );
        mPainter->drawLine( p4, p1 );
        }
      else if( smartMask & (snapApparentIntersect | snapIntersection) ) {
        //Крестик
        QPoint p1( p.x() - sm, p.y() - sm );
        QPoint p2( p.x() + sm, p.y() + sm );
        QPoint p3( p.x() - sm, p.y() + sm );
        QPoint p4( p.x() + sm, p.y() - sm );
        mPainter->drawLine( p1, p2 );
        mPainter->drawLine( p3, p4 );
        }
      else if( smartMask & snapMidPoint ) {
        //Треугольник
        QPoint p1( p.x() - sm, p.y() - sm );
        QPoint p2( p.x() + sm, p.y() - sm );
        QPoint p3( p.x(), p.y() + sm );
        mPainter->drawLine( p1, p2 );
        mPainter->drawLine( p2, p3 );
        mPainter->drawLine( p3, p1 );
        }
      else {
        //Кружок
        mPainter->drawEllipse( p, sm, sm );
        }
      //mPainter->setTransform( mTransform );
      }
    }
  }






//Draw cursor with current mode
void SdContext::drawCursor(SdPoint p)
  {
  if( SdEnvir::instance()->mCursorView != dcvNone ) {
    //Рисовать точку
    p = mTransform.map(p);
    //Setup pen
    mPainter->setPen( QPen( QBrush( SdEnvir::instance()->getSysColor(scCursor) ), 0, Qt::SolidLine ) );
    SdPoint a1,a2;
    SdRect r = mPainter->viewport();
    int w;
    //Draw cursor
    switch( SdEnvir::instance()->mCursorView ) {
      case dcvSmall :
        //X-line
        a1.set( p.x() - SdEnvir::instance()->mSmallCursorSize, p.y() );
        a2.set( p.x() + SdEnvir::instance()->mSmallCursorSize, p.y() );
        mPainter->drawLine( a1, a2 );
        //Y-line
        a1.set( p.x(), p.y() - SdEnvir::instance()->mSmallCursorSize );
        a2.set( p.x(), p.y() + SdEnvir::instance()->mSmallCursorSize );
        mPainter->drawLine( a1, a2 );
        break;
      case dcvSmall45 :
        a1.set( p.x() - SdEnvir::instance()->mSmallCursorSize, p.y() - SdEnvir::instance()->mSmallCursorSize );
        a2.set( p.x() + SdEnvir::instance()->mSmallCursorSize, p.y() + SdEnvir::instance()->mSmallCursorSize );
        mPainter->drawLine( a1, a2 );
        a1.set( p.x() + SdEnvir::instance()->mSmallCursorSize, p.y() - SdEnvir::instance()->mSmallCursorSize );
        a2.set( p.x() - SdEnvir::instance()->mSmallCursorSize, p.y() + SdEnvir::instance()->mSmallCursorSize );
        mPainter->drawLine( a1, a2 );
        break;
      case dcvFull :
        //X-line
        a1.set( 0, p.y() );
        a2.set( r.width(), p.y() );
        mPainter->drawLine( a1, a2 );
        //Y-line
        a1.set( p.x(), 0 );
        a2.set( p.x(), r.height() );
        mPainter->drawLine( a1, a2 );
        break;
      case dcvFull45 :
        w = qMax( r.width(), r.height() );
        a1.set( p.x() - w, p.y() - w );
        a2.set( p.x() + w, p.y() + w );
        mPainter->drawLine( a1, a2 );
        a1.set( p.x() + w, p.y() - w );
        a2.set( p.x() - w, p.y() + w );
        mPainter->drawLine( a1, a2 );
        break;
      }
    }
  }




void SdContext::setPen(int width, SdLayer *layer, int lineStyle )
  {
  setPen( width, convertColor(layer), lineStyle );
  }




void SdContext::setPen(int width, QColor color, int lineStyle)
  {
  //Convert line styles
  Qt::PenStyle style;
  if( lineStyle == dltDotted ) style = Qt::DotLine;
  else if( lineStyle == dltDashed ) style = Qt::DashLine;
  else style = Qt::SolidLine;

  int penWidthPix;
  if( mZeroOn && width == 0 )
    //When zero width line conversion is on, then draw zero line with setupped width
    penWidthPix = mZeroWidth;
  else
    penWidthPix = mScaler.phys2pixel(width);
  if( mPainter->isActive() )
    mPainter->setPen( QPen( QBrush( color ), penWidthPix, style, Qt::RoundCap, Qt::RoundJoin ) );
  }




void SdContext::setBrush(QColor color)
  {
  if( mPainter->isActive() ) {
    if( mOverOn )
      mPainter->setBrush( QBrush(mOverColor) );
    else
      mPainter->setBrush( QBrush(color) );
    }
  }




void SdContext::setProp( const SdPropLine &prop)
  {
  QColor color = convertColor(prop.mLayer.layer());
  setPen( prop.mWidth, color, prop.mType );
  setBrush( color );
  }





void SdContext::setFont(const SdPropText &prop)
  {
  QFont font( SdEnvir::instance()->getSysFont(prop.mFont) );
//  font.setPixelSize( qMax(mScaler.phys2pixel(prop.mSize.getValue()), 5) );
  font.setPixelSize( mScaler.phys2pixel(prop.mSize) );
  mPainter->setPen( convertColor(prop.mLayer.layer()) );
  mPainter->setFont( font );
  }




void SdContext::updateConverter()
  {
  if( mConverter ) {
    //calculate
    mTransform = mConverter->getFullMatrix();
    mPairLayer = mConverter->getFullPairLayer();
    mMirror    = mConverter->getMirror();
    mAngle     = mConverter->getAngle();
    mScaler.scaleSet( mConverter->getScale() );
    }
  else {
    mTransform.reset();
    mPairLayer = false;
    mMirror    = false;
    mAngle     = 0;
    mScaler.scaleSet(1.0);
    }

  //Setup painter
  //mPainter->setTransform( mTransform, false );
  }




QColor SdContext::convertColor(SdLayer *layer)
  {
  if( mOverOn )
    return mOverColor;
  if( mSelector )
    return SdEnvir::instance()->getSysColor(scSelected);
  return convertLayer(layer)->color();
  }
