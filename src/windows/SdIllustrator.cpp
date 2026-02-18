#include "SdIllustrator.h"
#include "objects/SdPoint.h"

#include <QDebug>

SdIllustrator::SdIllustrator() :
  minx(2000000000),
  miny(2000000000),
  maxx(-2000000000),
  maxy(-2000000000)
  {

  }




void SdIllustrator::drawLine(int x1, int y1, int x2, int y2, QColor color)
  {
  if( mPainter.isActive() ) {
    mPainter.setOpacity( 1.0 );
    mPainter.setPen( color );
    mPainter.drawLine( mapX(x1), mapY(y1), mapX(x2), mapY(y2) );
    }
  else {
    addPoint(x1,y1);
    addPoint(x2,y2);
    }
  }



void SdIllustrator::drawLineWidth(int width, int x1, int y1, int x2, int y2, QColor color)
  {
  if( mPainter.isActive() ) {
    mPainter.setOpacity( 1.0 );
    QPen pen( color, static_cast<double>(scale(width)) );
    pen.setCapStyle( Qt::RoundCap );
    mPainter.setPen( pen );
    mPainter.drawLine( mapX(x1), mapY(y1), mapX(x2), mapY(y2) );
//    drawFillCircle( x1, y1, width / 2, color );
//    drawFillCircle( x2, y2, width / 2, color );
    }
  else {
    addPoint(x1-width,y1-width);
    addPoint(x1+width,y1+width);
    addPoint(x2-width,y2-width);
    addPoint(x2+width,y2+width);
    }
  }





void SdIllustrator::drawLineArrow(int x1, int y1, int x2, int y2, QColor color, int arrowSize)
  {
  if( mPainter.isActive() ) {
    mPainter.setOpacity( 1.0 );
    mPainter.setPen( color );
    mPainter.drawLine( mapX(x1), mapY(y1), mapX(x2), mapY(y2) );
    //On point2 draw arrow with arrow size
    SdPvAngle angle = SdPoint(x1,y1).getAngle( SdPoint(x2,y2) );
    x2 = mapX(x2);
    y2 = mapY(y2);
    SdPoint e1( x2 + arrowSize, y2 - arrowSize / 2 );
    e1.rotate( SdPoint(x2,y2), angle );
    SdPoint e2( x2 + arrowSize, y2 + arrowSize / 2 );
    e2.rotate( SdPoint(x2,y2), angle );
    mPainter.drawLine( e1.x(), e1.y(), x2, y2 );
    mPainter.drawLine( e2.x(), e2.y(), x2, y2 );
    }
  else {
    addPoint(x1,y1);
    addPoint(x2,y2);
    }
  }

void SdIllustrator::drawLineDoubleArrow(int x1, int y1, int x2, int y2, QColor color, int arrowSize)
  {
  int xm = (x1 + x2) / 2;
  int ym = (y1 + y2) / 2;
  drawLineArrow( xm, ym, x1, y1, color, arrowSize );
  drawLineArrow( xm, ym, x2, y2, color, arrowSize );
  }




void SdIllustrator::drawRect(int x1, int y1, int x2, int y2, QColor color)
  {
  drawLine( x1, y1, x2, y1, color );
  drawLine( x2, y1, x2, y2, color );
  drawLine( x2, y2, x1, y2, color );
  drawLine( x1, y2, x1, y1, color );
  }



void SdIllustrator::drawCircle(int cx, int cy, int r, QColor color )
  {
  if( mPainter.isActive() ) {
    mPainter.setPen( color );
    mPainter.setBrush( QBrush(Qt::transparent) );
    mPainter.drawEllipse( mapX(cx-r), mapY(cy+r), scale(r*2), scale(r*2) );
    }
  else {
    addPoint(cx-r,cy-r);
    addPoint(cx+r,cy+r);
    }
  }




void SdIllustrator::drawCross(int cx, int cy, int r, QColor color)
  {
  drawLine( cx-r, cy-r, cx+r, cy+r, color );
  drawLine( cx-r, cy+r, cx+r, cy-r, color );
  }




void SdIllustrator::drawText(int x, int y, const QString str, QColor color, int size)
  {
  if( mPainter.isActive() ) {
    mPainter.setPen( color );
    mPainter.setOpacity( 1.0 );
    mPainter.setFont( QFont(QString("Arial"), size) );
    mPainter.drawText( mapX(x), mapY(y), str );
    }
  else {
    addPoint(x,y);
    }
  }




void SdIllustrator::drawTextMapped(int x, int y, const QString str, QColor color, int size)
  {
  drawText( x, y, str, color, scale(size) );
  }




void SdIllustrator::drawFillRect(int x1, int y1, int x2, int y2, QColor color, double opacity)
  {
  if( mPainter.isActive() ) {
    mPainter.setPen( color );
    mPainter.setBrush( QBrush(color) );
    mPainter.setOpacity( opacity );
    mPainter.drawRect( mapX(qMin(x1,x2)), mapY(qMax(y1,y2)), scale(abs(x2-x1)), scale(abs(y2-y1)) );
    qDebug() << mapX(qMin(x1,x2)) << mapY(qMax(y1,y2)) << scale(abs(x2-x1)) << scale(abs(y2-y1));
    }
  else {
    addPoint(x1,y1);
    addPoint(x2,y2);
    }
  }

void SdIllustrator::drawCenterFillRectWH(int xc, int yc, int w, int h, QColor color, double opacity)
  {
  drawFillRectWH( xc - w/2, yc - h/2, w, h, color, opacity );
  }




void SdIllustrator::drawFillRectWH(int x, int y, int w, int h, QColor color, double opacity)
  {
  drawFillRect( x, y, x + w, y + h, color, opacity );
  }






void SdIllustrator::drawFillCircle(int xc, int yc, int r, QColor color, double opacity)
  {
  if( mPainter.isActive() ) {
    mPainter.setPen( color );
    mPainter.setBrush( QBrush(color) );
    mPainter.setOpacity( opacity );
    mPainter.drawEllipse( mapX(xc-r), mapY(yc+r), scale(r*2), scale(r*2) );
    }
  else {
    addPoint(xc-r,yc-r);
    addPoint(xc+r,yc+r);
    }
  }








void SdIllustrator::setPixmap(int sizex, int sizey, QColor back )
  {
  mSizeX = sizex;
  mSizeY = sizey;
  mPixmap = QPixmap(sizex,sizey);
  double sx = maxx - minx;
  double sy = maxy - miny;
//  sx *= 1.1;
//  sy *= 1.1;
  if( sx > 0 && sy > 0 ) {
    mPixmap.fill(back);
    mPainter.begin( &mPixmap );
    double dsizex = static_cast<double>(sizex);
    double dsizey = static_cast<double>(sizey);
    mScale = qMin(dsizex / sx, dsizey / sy);
    minx -= static_cast<int>(dsizex / mScale - sx)  / 2;
    miny -= static_cast<int>(dsizey / mScale - sy ) / 2;
    mScale *= 0.98;
    }
  }





QPixmap SdIllustrator::pixmap()
  {
  if( mPainter.isActive() ) {
    mPainter.end();
    minx = miny = 2000000000;
    maxx = maxy = -2000000000;
    }
  return mPixmap;
  }




int SdIllustrator::scale(int val) const
  {
  return static_cast<int>( static_cast<double>(val) * mScale );
  }




int SdIllustrator::mapX(int x) const
  {
  return scale( x - minx );
  }




int SdIllustrator::mapY(int y) const
  {
  return mSizeY - scale( y - miny ) - 4;
  }




void SdIllustrator::addPoint(int x, int y)
  {
  minx = qMin(minx,x);
  miny = qMin(miny,y);
  maxx = qMax(maxx,x);
  maxy = qMax(maxy,y);
  }
