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
#include <QPainter>


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

void SdContext::line(SdPoint a, SdPoint b, SdPropLine &prop)
  {
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    setPen( prop.mWidth, prop.mLayer.layer(), prop.mType );
    mPainter->drawLine( a, b );
    }
  }

void SdContext::rect(SdQuadrangle q, SdPropLine &prop)
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




void SdContext::rectSelect( SdRect r, QColor color )
  {
  mPainter->setPen( QPen( QBrush(color), 0, Qt::DotLine ) );
  mPainter->drawLine( r.topLeft(), r.topRight() );
  mPainter->drawLine( r.topRight(), r.bottomRight() );
  mPainter->drawLine( r.bottomRight(), r.bottomLeft() );
  mPainter->drawLine( r.bottomLeft(), r.topLeft() );
  }



void SdContext::region(SdPointList &points, SdPropLine &prop, bool autoClose)
  {
  if( mSelector || prop.mLayer.layer(mPairLayer)->isVisible() ) {
    if( points.count() > 1 ) {
      setPen( prop.mWidth, prop.mLayer.layer(), prop.mType );
      for( int i = 1; i < points.count(); i++ )
        mPainter->drawLine( points.at(i-1), points.at(i) );
      if( autoClose && points.first() != points.last() )
        mPainter->drawLine( points.first(), points.last() );
      }
    }

  }

void SdContext::dotTrase(SdPoint p)
  {
  Q_UNUSED(p)
  }

void SdContext::setPen(int width, SdLayer *layer, int lineStyle )
  {
  //Convert line styles
  Qt::PenStyle style;
  if( lineStyle == dltDotted ) style = Qt::DotLine;
  else if( lineStyle == dltDashed ) style = Qt::DashLine;
  else style = Qt::SolidLine;

  mPainter->setPen( QPen( QBrush( convertColor(layer) ), width, style ) );
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
