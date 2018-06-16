#include "windows/SdIllustrator.h"
#include <QApplication>
#include <QLabel>
#include <QPolygon>
#include <QDebug>





void drawPadSchematic( SdIllustrator &ill, bool isCircle, bool hole, bool array ) {
  //pcb
  ill.drawFillRect( 0,0, 250,250, QColor(0x007F0E) );

  if( isCircle ) {
    //pad
    ill.drawFillCircle( 125,125, 50, QColor(0xFFE97F) );

    //Hole if present
    if( hole )
      ill.drawFillCircle( 125,125, 20, QColor(0x7F6A00) );

    //mask
    ill.drawFillCircle( 125,125, 60, QColor(0xA5FF7F), 0.5 );

    //Stensil if present
    if( !hole )
      ill.drawFillCircle( 125,125, 40, QColor(0x808080) );
    }
  else {
    //pad
    ill.drawFillRectWH( 125-60,125-50, 120,100, QColor(0xFFE97F) );

    //Hole if present
    if( hole )
      ill.drawFillCircle( 125,125, 20, QColor(0x7F6A00) );

    //mask
    ill.drawFillRectWH( 125-70,125-60, 140,120, QColor(0xA5FF7F), 0.5 );

    //Stensil if present
    if( !hole ) {
      if( array ) {
        ill.drawFillRectWH( 125-50,125-40, 40,30, QColor(0x808080) );
        ill.drawFillRectWH( 125+50-40,125-40, 40,30, QColor(0x808080) );
        ill.drawFillRectWH( 125-50,125+40-30, 40,30, QColor(0x808080) );
        ill.drawFillRectWH( 125+50-40,125+40-30, 40,30, QColor(0x808080) );
        }
      else
        ill.drawFillRectWH( 125-50,125-40, 100,80, QColor(0x808080) );
      }
    }
  }




int main(int argc, char *argv[])
  {
  QApplication a(argc, argv);

  QPolygon pol;
  pol << QPoint(-100,-10) << QPoint(100,-10) << QPoint(100,10) << QPoint(-100,10);
  qDebug() << pol;
  qDebug() << pol.containsPoint( QPoint(0,0), Qt::OddEvenFill );
  QPolygon win;
  win << QPoint(-10,-200) << QPoint(10,-200) << QPoint(10,200) << QPoint(-10,200);
  pol = pol.subtracted( win );
  qDebug() << pol;
  qDebug() << pol.containsPoint( QPoint(0,0), Qt::OddEvenFill );

//  SdIllustrator ill;
//  draw( ill, false, false, true );
//  ill.setPixmap( 250, 250, Qt::white );
//  draw( ill, false, false, true );

//  QLabel lab;
//  lab.setPixmap( ill.pixmap() );
//  lab.show();

  //return a.exec();
  return 0;
  }
