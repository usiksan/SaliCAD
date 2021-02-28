#include "Sd3dDraw.h"
#include "master3d/SdM3dFunRegionCircle.h"
//#include "master3d/SdM3dFunRegionTranslate.h"
#include <QColor>




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
  SdM3dRegion region = SdM3dFunRegionCircle::regionCircle( static_cast<float>(radius) / 1000.0, 1.0, QVector3D( center.xmm(), center.ymm(), z ) );
  f->glBegin( GL_LINES );
  for( int i = 0; i < region.count(); i++ ) {
    QVector3D start = region.at(i);
    QVector3D finish = i + 1 < region.count() ? region.at(i + 1) : region.first();
    f->glVertex3f( start.x(), start.y(), start.z() );
    f->glVertex3f( finish.x(), finish.y(), finish.z() );
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
  SdM3dRegion region = SdM3dFunRegionCircle::regionCircle( static_cast<float>(radius) / 1000.0, 1.0, QVector3D( center.xmm(), center.ymm(), z ) );
  f->glBegin(GL_POLYGON);
  f->glNormal3f( 0, 0, 1.0 );
  for( auto const &v : region ) {
    f->glVertex3f( v.x(), v.y(), v.z() );
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
  fcolor[0] = col.redF();
  fcolor[1] = col.greenF();
  fcolor[2] = col.blueF();
  fcolor[3] = col.alphaF();
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fcolor );
  }



void Sd3dDraw::flatPanel(QOpenGLFunctions_2_0 *f, SdPointList list, int z, unsigned color)
  {
  if( list.count() ) {
    float fz(z);
    fz /= 1000.0;
    //Top surface
    f->glColor3b( qRed(color) >> 1, qGreen(color) >> 1, qBlue(color) >> 1 );
    f->glBegin(GL_POLYGON);
    f->glNormal3f( 0, 0, 1 );
    for( const QPoint &p : list ) {
      QPointF fp(p);
      fp /= 1000.0;
      f->glVertex3f( fp.x(), fp.y(), 0.0 );
      }
    f->glEnd();

    //Bottom surface
    f->glBegin(GL_POLYGON);
    f->glNormal3f( 0, 0, -1 );
    for( const QPoint &p : list ) {
      QPointF fp(p);
      fp /= 1000.0;
      f->glVertex3f( fp.x(), fp.y(), fz );
      }
    f->glEnd();

    //Side surfaces
    for( int i = 0; i < list.count(); i++ ) {
      QPointF fp1( list.at(i) );
      QPointF fp2;
      if( i == list.count() - 1 ) fp2 = list.at(0);
      else fp2 = list.at( i + 1 );
      fp1 /= 1000.0;
      fp2 /= 1000.0;
      f->glBegin(GL_POLYGON);
      f->glVertex3f( fp1.x(), fp1.y(), 0.0 );
      f->glVertex3f( fp2.x(), fp2.y(), 0.0 );
      f->glVertex3f( fp2.x(), fp2.y(), fz );
      f->glVertex3f( fp1.x(), fp1.y(), fz );
      f->glEnd();
      }
    }
  }

