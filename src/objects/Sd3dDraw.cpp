/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Utilites for common 3d drawing
*/
#include "Sd3dDraw.h"
#include "SdEnvir.h"

#include <QColor>


static void drawPolygon( QOpenGLFunctions_2_0 *f, Sd3dRegion region )
  {
  QVector3D normal = QVector3D::normal( region.at(0), region.at(1), region.at(2) );
  f->glBegin(GL_POLYGON);
  f->glNormal3f( normal.x(), normal.y(), normal.z() );
  for( auto const &v : region ) {
    f->glVertex3f( v.x(), v.y(), v.z() );
    }
  f->glEnd();
  }





//!
//! \brief line Draw line segment
//! \param f    OpenGL
//! \param a    Start point of segment
//! \param b    Finish point of segment
//! \param z    z level in mm
//!
void Sd3dDraw::line(QOpenGLFunctions_2_0 *f, SdPoint a, SdPoint b, float z)
  {
  f->glBegin( GL_LINES );
  f->glVertex3f( a.xmm(), a.ymm(), z );
  f->glVertex3f( b.xmm(), b.ymm(), z );
  f->glEnd();
  }





//!
//! \brief rect Draw rectangle region with 4 lines
//! \param f    OpenGL
//! \param a    First corner point
//! \param b    Opposite corner point
//! \param z    z level in mm
//!
void Sd3dDraw::rect(QOpenGLFunctions_2_0 *f, SdPoint a, SdPoint b, float z)
  {
  f->glBegin( GL_LINES );
  f->glVertex3f( a.xmm(), a.ymm(), z );
  f->glVertex3f( a.xmm(), b.ymm(), z );

  f->glVertex3f( a.xmm(), b.ymm(), z );
  f->glVertex3f( b.xmm(), b.ymm(), z );

  f->glVertex3f( b.xmm(), b.ymm(), z );
  f->glVertex3f( b.xmm(), a.ymm(), z );

  f->glVertex3f( b.xmm(), a.ymm(), z );
  f->glVertex3f( a.xmm(), a.ymm(), z );

  f->glEnd();
  }





//!
//! \brief rect Draw filled rectangle polygon
//! \param f    OpenGL
//! \param a    First corner point
//! \param b    Opposite corner point
//! \param z    z level in mm
//!
void Sd3dDraw::rectFilled(QOpenGLFunctions_2_0 *f, SdPoint a, SdPoint b, float z)
  {
  f->glBegin(GL_POLYGON);
  f->glNormal3f( 0, 0, 1.0 );
  f->glVertex3f( a.xmm(), a.ymm(), z );
  f->glVertex3f( a.xmm(), b.ymm(), z );
  f->glVertex3f( b.xmm(), b.ymm(), z );
  f->glVertex3f( b.xmm(), a.ymm(), z );
  f->glEnd();
  }






//!
//! \brief circle Draw circle as mulcorner region
//! \param f      OpenGL
//! \param center Center of circle
//! \param radius Radius of circle in mcm
//! \param z      z level in mm
//!
void Sd3dDraw::circle(QOpenGLFunctions_2_0 *f, SdPoint center, int radius, float z)
  {
//  Sd3dRegion region = sd3dRegionCircle( static_cast<float>(radius) / 1000.0, 2.0, QVector3D( center.xmm(), center.ymm(), z ) );
//  f->glBegin( GL_LINES );
//  for( int i = 0; i < region.count(); i++ ) {
//    QVector3D start = region.at(i);
//    QVector3D finish = i + 1 < region.count() ? region.at(i + 1) : region.first();
//    f->glVertex3f( start.x(), start.y(), start.z() );
//    f->glVertex3f( finish.x(), finish.y(), finish.z() );
//    }
//  f->glEnd();
  }





//!
//! \brief circle Draw circle as filled multicorner polygon
//! \param f      OpenGL
//! \param center Center of circle
//! \param radius Radius of circle in mcm
//! \param z      z level in mm
//!
void Sd3dDraw::circleFill(QOpenGLFunctions_2_0 *f, SdPoint center, int radius, float z)
  {
//  Sd3dRegion region = sd3dRegionCircle( static_cast<float>(radius) / 1000.0, 2.0, QVector3D( center.xmm(), center.ymm(), z ) );
//  drawPolygon( f, region );
  }





//!
//! \brief color Setup current color
//! \param f     OpenGL
//! \param col   New current color
//!
void Sd3dDraw::color(QOpenGLFunctions_2_0 *f, QColor col)
  {
  float fcolor[4];
  colorToFloat( col, fcolor );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fcolor );
  }



//!
//! \brief colorToFloat Convert QColor to array of four float color components
//! \param col          Source color
//! \param fcolor       Destignation array of float, must be at least 4 item
//!
void Sd3dDraw::colorToFloat(QColor col, float *fcolor)
  {
  fcolor[0] = col.redF();
  fcolor[1] = col.greenF();
  fcolor[2] = col.blueF();
  fcolor[3] = col.alphaF();
  }








//!
//! \brief flatPanel Draws flat panel (often pcb) by contour point list and faces color
//! \param f         OpenGL
//! \param list      Contour point list
//! \param z         Thickness of flat panel in micron (negativ extruded in down)
//! \param color     Color of flat panel
//!
void Sd3dDraw::flatPanel(QOpenGLFunctions_2_0 *f, SdPointList list, int z, QColor color)
  {
//  Sd3dRegion region;
//  region.reserve( list.count() );
//  float fz(z);
//  fz /= 1000.0;
//  fz -= 0.22;
//  for( auto const &p : list ) {
//    float x(p.x());
//    x /= 1000.0;
//    float y(p.y());
//    y /= 1000.0;
//    QVector3D v( x, y, -0.11 );
//    region.append( v );
//    }
//  Sd3dModel pcb = sd3dModelExtrude( region, -fz, color );
//  drawModel( f, pcb );
  }





//!
//! \brief drawModel Draw model in 3d world
//! \param f         OpenGL
//! \param model     Model to draw
//!
void Sd3dDraw::drawModel(QOpenGLFunctions_2_0 *f, const Sd3dModel &model)
  {
  model.draw3d( f );
  }

