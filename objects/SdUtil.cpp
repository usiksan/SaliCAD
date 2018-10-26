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


//Extract value from string
double SdUtil::extractValueFromString(const QString str, int &start)
  {
  QString val;
  while( start < str.length() ) {
    QChar ch = str.at(start);
    if( ch == QChar('-') ) val += ch;
    else if( ch == QChar(',') || ch == QChar('.') ) val += QChar('.');
    else if( ch.isDigit() ) val += ch;
    else
      //Extracting completed
      break;
    start++;
    }
  if( val.length() ) return val.toDouble();
  return 0;
  }



//Extract logical value from string
int SdUtil::extractLogFromString(const QString str, int &start, double ppm)
  {
  return phys2log( extractValueFromString( str, start ), ppm );
  }



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





//Convert logical um int coord to mm minimum string
QString SdUtil::um2mm(int log)
  {
  double phys = log2phys( log, 0.001 );
  int dec = 0;
  if( log % 10 ) dec = 3;
  else if( log % 100 ) dec = 2;
  else if( log % 1000 ) dec = 1;

  return QString::number( phys, 'f', dec );
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
QPolygonF SdUtil::octagon(int centerx, int centery, int radius)
  {
  int d = radius * 41 / 100;
  QPolygonF pgn;
  pgn << QPointF(centerx - radius, centery - d )
      << QPointF(centerx - d,      centery - radius )
      << QPointF(centerx + d,      centery - radius )
      << QPointF(centerx + radius, centery - d )
      << QPointF(centerx + radius, centery + d )
      << QPointF(centerx + d,      centery + radius )
      << QPointF(centerx - d,      centery + radius )
      << QPointF(centerx - radius, centery + d );
  return pgn;
  }




int SdUtil::iLimit(int val, int vmin, int vmax)
  {
  if( val < vmin ) return vmin;
  if( val > vmax ) return vmax;
  return val;
  }



