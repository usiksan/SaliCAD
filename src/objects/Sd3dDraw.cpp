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
#include <math.h>




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
  QPointF centerf(center);
  centerf /= 1000.0;
  float radiusf( static_cast<float>(radius) / 1000.0 );
  //Build circle with step degree
  float startx = centerf.x();
  float starty = centerf.y() + radiusf;
  f->glBegin( GL_LINES );
  for( float angleDegree = 10.0; angleDegree <= 360.0; angleDegree += 10.0 ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    float stopx = centerf.x() + sin(angle) * radiusf;
    float stopy = centerf.y() + cos(angle) * radiusf;
    f->glVertex3f( startx, starty, z );
    f->glVertex3f( stopx, stopy, z );
    }
  f->glEnd();
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
  QPointF centerf(center);
  centerf /= 1000.0;
  float radiusf( static_cast<float>(radius) / 1000.0 );
  //Build circle with step degree
  f->glBegin( GL_POLYGON );
  f->glNormal3f( 0, 0, 1.0 );
  for( float angleDegree = 0; angleDegree < 360.0; angleDegree += 10.0 ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    float stopx = centerf.x() + sin(angle) * radiusf;
    float stopy = centerf.y() + cos(angle) * radiusf;
    f->glVertex3f( stopx, stopy, z );
    }
  f->glEnd();
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
//! \brief drawModel Draw model in 3d world
//! \param f         OpenGL
//! \param model     Model to draw
//!
void Sd3dDraw::drawModel(QOpenGLFunctions_2_0 *f, const Sd3dModel &model)
  {
  model.draw3d( f );
  }

