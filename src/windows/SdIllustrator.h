#ifndef SDILLUSTRATOR_H
#define SDILLUSTRATOR_H

#include <QColor>
#include <QPixmap>
#include <QPainter>

class SdIllustrator
  {
    int minx;
    int miny;
    int maxx;
    int maxy;

    double   mScale;
    int      mSizeX;
    int      mSizeY;

    QPixmap  mPixmap;
    QPainter mPainter;
  public:
    SdIllustrator();

    //Linear
    void    drawLine( int x1, int y1, int x2, int y2, QColor color );
    void    drawLineWidth( int width, int x1, int y1, int x2, int y2, QColor color );
    void    drawLineArrow( int x1, int y1, int x2, int y2, QColor color, int arrowSize = 10 );
    void    drawLineDoubleArrow( int x1, int y1, int x2, int y2, QColor color, int arrowSize = 10 );
    void    drawRect( int x1, int y1, int x2, int y2, QColor color );
    void    drawCircle(int cx, int cy, int r, QColor color );
    void    drawCross( int cx, int cy, int r, QColor color );
    void    drawText(int x, int y, const QString str, QColor color, int size = 12 );
    void    drawTextMapped(int x, int y, const QString str, QColor color, int size );

    //Filled
    void    drawFillRect( int x1, int y1, int x2, int y2, QColor color, double opacity = 1.0 );
    void    drawCenterFillRectWH( int xc, int yc, int w, int h, QColor color, double opacity = 1.0 );
    void    drawFillRectWH( int x, int y, int w, int h, QColor color, double opacity = 1.0 );
    void    drawFillCircle( int xc, int yc, int r, QColor color, double opacity = 1.0 );

    void    setPixmap(int sizex, int sizey , QColor back);
    QPixmap pixmap();

    int     getMinX() const { return minx; }
    int     getMinY() const { return miny; }
    int     getMaxX() const { return maxx; }
    int     getMayY() const { return maxy; }

  private:
    int     scale( int val ) const;
    int     mapX( int x ) const;
    int     mapY( int y ) const;
    void    addPoint( int x, int y );
  };

#endif // SDILLUSTRATOR_H
