/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Some utils
*/
#include "SdUtil.h"
#include <QDateTime>


//Convert textual representation of physical coords to logical int
int SdUtil::phys2log(const QString src, double ppm)
  {
  return phys2log( str2phys(src), ppm );
  }

int SdUtil::phys2log(double phys, double ppm)
  {
  if( ppm == 0 ) ppm = 1.0;
  return static_cast<int>((phys + ppm/2) / ppm);
  }

QPoint SdUtil::phys2log(QPointF phys, double ppm)
  {
  QPoint p;
  p.rx() = phys2log( phys.x(), ppm );
  p.ry() = phys2log( phys.y(), ppm );
  return p;
  }




//Convert textual representation of physical coord to double phys
double SdUtil::str2phys(const QString src)
  {
  QString txt = src;
  txt.replace( QChar(','), QString(".") );
  return txt.toDouble();
  }




//Convert logical int coords to textual physical
QString SdUtil::log2physStr( int log, double ppm )
  {
  double phys = log2phys( log, ppm );

  return QString::number( phys, 'f', ppm < 0.1 ? 3 : 1 );
  }




//Convert logical int coord to physical
double SdUtil::log2phys(int log, double ppm)
  {
  return static_cast<double>(log) * ppm;
  }



//Convert logical point to physical
QPointF SdUtil::log2phys(QPoint log, double ppm)
  {
  QPointF p;
  p.rx() = log2phys( log.x(), ppm );
  p.ry() = log2phys( log.y(), ppm );
  return p;
  }




//Get current time from 2000year
int SdUtil::getTime2000()
  {
  return static_cast<int>( QDateTime::currentDateTimeUtc().toSecsSinceEpoch() - timeOffsetConstant );
  }



//Convert time from 2000year to time from epoch
qint64 SdUtil::time2000toEpoch(int time)
  {
  return timeOffsetConstant + static_cast<qint64>(time);
  }




//Test if string contains only english symbols
bool SdUtil::isEnglish(const QString str)
  {
  for( QChar ch : str ) {
    if( !ch.toLatin1() )
      return false;
    }
  return true;
  }



//Swap int
void SdUtil::swapInt(int &src, int *dst)
  {
  if( dst != nullptr ) {
    int tmp = *dst;
    *dst = src;
    src = tmp;
    }
  }



//Swap bool
void SdUtil::swapBool(bool &src, bool *dst)
  {
  if( dst != nullptr ) {
    bool tmp = *dst;
    *dst = src;
    src = tmp;
    }
  }




//Create octagon over circle
QPolygon SdUtil::octagon(int centerx, int centery, int radius)
  {
  int d = radius * 41 / 100;
  QPolygon pgn;
  pgn << SdPoint(centerx - radius, centery - d )
      << SdPoint(centerx - d,      centery - radius )
      << SdPoint(centerx + d,      centery - radius )
      << SdPoint(centerx + radius, centery - d )
      << SdPoint(centerx + radius, centery + d )
      << SdPoint(centerx + d,      centery + radius )
      << SdPoint(centerx - d,      centery + radius )
      << SdPoint(centerx - radius, centery + d );
  return pgn;
  }


