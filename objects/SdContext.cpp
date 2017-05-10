/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdContext.h"
#include "SdConverter.h"
#include "SdEnvir.h"
#include "SdConverterText.h"
#include <QPainter>
#include <QFont>


SdContext::SdContext(SdPoint grid, QPainter *painter) :
  mPainter(painter),
  mConverter(0),
  mGrid(grid),
  mSelector(0),
  mTransform(),
  mPairLayer(false)
  {

  }

SdContext *SdContext::setConverter(SdConverter *c)
  {
  //Append converter to stack
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

void SdContext::line(SdPoint a, SdPoint b)
  {
  mPainter->drawLine( a, b );
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
  if( mSelector || (layer != 0 && layer->isVisible()) ) {
    cross( a, sdEnvir->mSymPinSize, convertColor( layer ) );
    }
  }




void SdContext::partPin(SdPoint a, SdLayer *layer)
  {
  if( mSelector || (layer != 0 && layer->isVisible()) ) {
    setPen( 0, convertColor( layer ), dltSolid );
    circle( a, sdEnvir->mPrtPinSize );
    }
  }





void SdContext::cross(SdPoint a, int size, QColor color)
  {
  setPen( 0, color, dltSolid );
//  mPainter->setPen( QPen( QBrush(color), 0 ) );
  line( SdPoint(a.x()-size,a.y()-size), SdPoint(a.x()+size,a.y()+size ) );
  line( SdPoint(a.x()+size,a.y()-size), SdPoint(a.x()-size,a.y()+size ) );
  }




void SdContext::quadrangle(SdQuadrangle q, const SdPropLine &prop)
  {
  //Draw 4 edges
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    setPen( prop.mWidth, prop.mLayer.layer(), prop.mType );
    mPainter->drawLine( q.p1, q.p2 );
    mPainter->drawLine( q.p2, q.p3 );
    mPainter->drawLine( q.p3, q.p4 );
    mPainter->drawLine( q.p4, q.p1 );
    }
  }




void SdContext::rect(SdRect r)
  {
  mPainter->drawLine( r.topLeft(), r.topRight() );
  mPainter->drawLine( r.topRight(), r.bottomRight() );
  mPainter->drawLine( r.bottomRight(), r.bottomLeft() );
  mPainter->drawLine( r.bottomLeft(), r.topLeft() );
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
  mPainter->drawRect(r);
  }




void SdContext::fillRect(SdRect r, const SdPropLine &prop)
  {
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    QColor color = convertColor(prop.mLayer.layer());
    mPainter->setPen( color );
    setBrush( color );
    fillRect( r );
    }
  }




void SdContext::circle(SdPoint center, int radius)
  {
  mPainter->setBrush( QBrush(Qt::transparent) );
  mPainter->drawEllipse( center, radius, radius );
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
  mPainter->drawEllipse( center, radius, radius );
  }



void SdContext::circleFill(SdPoint center, int radius, const SdPropLine &prop)
  {
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    QColor color = convertColor(prop.mLayer.layer());
    mPainter->setPen( color );
    setBrush( color );
    circleFill( center, radius );
    }
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

  SdConverterText cnv( this, pos, dir );
  setConverter( &cnv );

  if( !str.isEmpty() )
    mPainter->drawText( over, Qt::AlignLeft | Qt::AlignTop, str );

  if( cp1 && cp2 ) {
    if( cursor == 0 ) {
      *cp1 = cnv.getMatrix().map( over.topLeft() );
      *cp2 = cnv.getMatrix().map( over.bottomLeft() );
      }
    else {
      QRect rpos = mPainter->boundingRect( over, Qt::AlignLeft | Qt::AlignTop, str.left(cursor) );
      *cp1 = cnv.getMatrix().map( rpos.topRight() );
      *cp2 = cnv.getMatrix().map( rpos.bottomRight() );
      }
    }

  if( sel ) {
    //Fill selection rect
    QRect rStart = mPainter->boundingRect( over, Qt::AlignLeft | Qt::AlignTop, str.left(start) );
    QRect rStop  = mPainter->boundingRect( over, Qt::AlignLeft | Qt::AlignTop, str.left(stop) );
    QRect res;
    res.setTopLeft( rStart.topRight() );
    res.setBottomRight( rStop.bottomRight() );
    sel->set( cnv.getMatrix().mapRect( res ) );
    }

  over.set( cnv.getMatrix().mapRect( over ) );
  }




void SdContext::text( SdPoint pos, SdRect &over, const QString str, const SdPropText &prop)
  {
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    setFont( prop );
    textEx( pos, over, str, prop.mDir.getValue(), prop.mHorz.getValue(), prop.mVert.getValue() );
    }
  }







void SdContext::region(const SdPointList &points, bool autoClose)
  {
  for( int i = 1; i < points.count(); i++ )
    mPainter->drawLine( points.at(i-1), points.at(i) );
  if( autoClose && points.first() != points.last() )
    mPainter->drawLine( points.first(), points.last() );
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




void SdContext::smartPoint(SdPoint a, int smartMask)
  {
  if( smartMask ) {
    //Рисовать точку
    QPoint p = mTransform.map(a);
    if( a.x() >= 0 && a.x() <= mPainter->device()->width() &&
        a.y() >= 0 && a.y() <= mPainter->device()->height() ) {
      //Точка попадает в область видимости - рисуем
      mPainter->resetTransform();

      //Подготовить перо
      mPainter->setPen( QPen(QBrush(sdEnvir->getSysColor(scSmart)), sdEnvir->mSmartWidth) );
      mPainter->setBrush( Qt::transparent );
      int sm = sdEnvir->mSmartSize;

      //Приступить к рисованию
      if( smartMask & (snapNearestNet | snapNearestPin | snapNextPin | snapNetPoint) ) {
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
        mPainter->drawEllipse( a, sm, sm );
        mPainter->drawLine( p1, p2 );
        }
      else if( smartMask & snapQuadrant ) {
        //Кружок и четыре точки по квадрантам
        mPainter->drawEllipse( a, sm, sm );
        mPainter->drawEllipse( QPoint(p.x() + sm, p.y()), 1, 1 );
        mPainter->drawEllipse( QPoint(p.x() - sm, p.y()), 1, 1 );
        mPainter->drawEllipse( QPoint(p.x(), p.y() + sm), 1, 1 );
        mPainter->drawEllipse( QPoint(p.x(), p.y() - sm), 1, 1 );
        }
      else if( smartMask & snapCenter ) {
        //Кружок
        mPainter->drawEllipse( a, sm, sm );
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
        mPainter->drawEllipse( a, sm, sm );
        }
      mPainter->setTransform( mTransform );
      }
    }
  }




void SdContext::dotTrase(SdPoint p)
  {
  Q_UNUSED(p)
  //TODO dotTrase
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

  mPainter->setPen( QPen( QBrush( color ), width, style ) );
  }




void SdContext::setBrush(QColor color)
  {
  mPainter->setBrush( QBrush(color) );
  }




void SdContext::setProp( const SdPropLine &prop)
  {
  QColor color = convertColor(prop.mLayer.layer());
  setPen( prop.mWidth, color, prop.mType );
  setBrush( color );
  }





void SdContext::setFont(const SdPropText &prop)
  {
  QFont font( sdEnvir->getSysFont(prop.mFont.getValue()) );
  font.setPixelSize( prop.mSize.getValue() );
  mPainter->setPen( convertColor(prop.mLayer.layer()) );
  mPainter->setFont( font );
  }




void SdContext::updateConverter()
  {
  if( mConverter ) {
    //calculate
    mTransform = mConverter->getFullMatrix();
    mPairLayer = mConverter->getFullPairLayer();
    }
  else {
    mTransform.reset();
    mPairLayer = false;
    }

  //Setup painter
  mPainter->setTransform( mTransform, false );
  }

QColor SdContext::convertColor(SdLayer *layer)
  {
  if( mSelector )
    return sdEnvir->getSysColor(scSelected);
  return convertLayer(layer)->color();
  }
