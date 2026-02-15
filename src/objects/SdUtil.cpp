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



//!
//! \brief polygonOverCircle Creates regular polygon circumscribed around circle
//! \param centerx           Center X coordinate
//! \param centery           Center Y coordinate
//! \param radius            Circle radius
//! \param sides             Number of polygon sides (minimum 3)
//! \return                  Polygon points in clockwise order
//!
QPolygonF SdUtil::polygonOverCircle(int centerx, int centery, int radius, int sides)
  {
  QPolygonF pgn;
  if( sides < 3 )
    return pgn;

  double step = 2 * M_PI / sides;
  double startAngle = M_PI / 2;  // Start from top point

  for( int i = 0; i < sides; i++ ) {
    double angle = startAngle - i * step;  // Clockwise direction
    double x = centerx + radius * cos(angle);
    double y = centery - radius * sin(angle);  // Qt Y axis goes down
    pgn << QPointF(x, y);
    }

  return pgn;
  }





//!
//! \brief roundrect Creates polygon for rectangle with rounded corners
//! \param r         Rectangle in integer coordinates (SdPoint system: Y up)
//! \param ratio     Rounding ratio in internal units (same as radius)
//! \param sides     Total number of points in polygon (should be multiple of 4)
//!                  Minimum 4 (just rectangle), recommended 16-32 for smooth corners
//! \return          Polygon approximating rounded rectangle with specified total points
//!                  Points are in SdPoint coordinate system (Y up)
//!
QPolygonF SdUtil::roundrect(QRect r, int ratio, int sides)
  {
  QPolygonF pgn;

  // Ensure sides is at least 4 and adjust to be multiple of 4
  if( sides < 4 )
    sides = 16;  // Default to 16 if too small

  // Round to nearest multiple of 4
  int remainder = sides % 4;
  if( remainder != 0 )
    sides += (4 - remainder); // Round up

  // Get rectangle dimensions
  int left   = r.left();
  int top    = r.top();      // Top is smaller Y (since Y goes up)
  int right  = r.right();
  int bottom = r.bottom();   // Bottom is larger Y (since Y goes up)
  int width  = r.width();
  int height = r.height();

  // Calculate radius from ratio (ratio is in thousandths)
  int radius = qMin(width, height) * qBound(0,ratio,500) / 1000;


  if( radius == 0 ) {
    int segmentsPerSide = sides / 4;  // Number of points on each side (including corners)
    // Simple rectangle - distribute points evenly along each side
    for( int i = 0; i <= segmentsPerSide; i++ ) {
      // Top side (left to right)
      double x = left + (right - left) * i / segmentsPerSide;
      pgn << QPointF(x, top);
      }

    for( int i = 1; i <= segmentsPerSide; i++ ) {
      // Right side (top to bottom)
      double y = top + (bottom - top) * i / segmentsPerSide;
      pgn << QPointF(right, y);
      }

    for( int i = 1; i <= segmentsPerSide; i++ ) {
      // Bottom side (right to left)
      double x = right - (right - left) * i / segmentsPerSide;
      pgn << QPointF(x, bottom);
      }

    for( int i = 1; i <= segmentsPerSide - 1; i++ ) {
      // Left side (bottom to top)
      double y = bottom - (bottom - top) * i / segmentsPerSide;
      pgn << QPointF(left, y);
      }

    return pgn;
    }

  int segmentsPerCorner = sides / 4;  // Number of points per corner (including endpoints)
  double step = 90.0 / (segmentsPerCorner - 1);  // Steps between points in degrees
  double rad = radius;

  // Helper lambda to add point with proper angle
  auto addCornerPoint = [&](double angle, double centerX, double centerY) {
    double x = centerX + rad * cos(angle * M_PI / 180.0);
    double y = centerY + rad * sin(angle * M_PI / 180.0);  // + sin because Y up
    pgn << QPointF(x, y);
    };

  // Bottom-right corner (270° to 360°) - start from bottom-most point
  for( int i = 0; i <= segmentsPerCorner; i++ ) {
    double angle = 270 + i * step;
    addCornerPoint(angle, right - rad, bottom - rad);
    }

  // Bottom-left corner (180° to 270°)
  for( int i = 1; i <= segmentsPerCorner; i++ ) {
    double angle = 180 + i * step;
    addCornerPoint(angle, left + rad, bottom - rad);
    }

  // Top-left corner (90° to 180°)
  for( int i = 1; i <= segmentsPerCorner; i++ ) {
    double angle = 90 + i * step;
    addCornerPoint(angle, left + rad, top + rad);
    }

  // Top-right corner (0° to 90°)
  for( int i = 1; i < segmentsPerCorner - 1; i++ ) {  // -1 to avoid duplicating start point
    double angle = i * step;
    addCornerPoint(angle, right - rad, top + rad);
    }

  return pgn;
  }



//!
//! \brief ellipse Creates polygon approximation of ellipse
//! \param r       Rectangle defining ellipse bounds
//! \param sides   Total number of points in polygon (minimum 4, recommended 16-32)
//! \return        Polygon approximating the ellipse with specified number of points
//!
QPolygonF SdUtil::ellipse(QRect r, int sides)
  {
  QPolygonF poly;

  if( sides < 4 )
    sides = 16;  // Default to 16 if invalid

  int centerX = r.center().x();
  int centerY = r.center().y();
  int a = r.width() / 2;   // Semi-major axis
  int b = r.height() / 2;  // Semi-minor axis

  for( int i = 0; i < sides; i++ ) {
    double angle = 2 * M_PI * i / sides;
    double x = centerX + a * cos(angle);
    double y = centerY + b * sin(angle);  // Y up system
    poly << QPointF(x, y);
    }

  return poly;
  }



int SdUtil::iLimit(int val, int vmin, int vmax)
  {
  if( val < vmin ) return vmin;
  if( val > vmax ) return vmax;
  return val;
  }



