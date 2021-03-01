#include "Sd3dDraw.h"
#include "master3d/SdM3dFunRegionCircle.h"
#include "master3d/SdM3dFunRegionRect.h"
#include "master3d/SdM3dFunModelHole.h"
#include "master3d/SdM3dFunModelTranslate.h"
#include "master3d/SdM3dFunModelExtrude.h"
#include <QColor>


static void drawPolygon( QOpenGLFunctions_2_0 *f, SdM3dRegion region )
  {
  QVector3D normal = QVector3D::normal( region.at(0), region.at(1), region.at(2) );
  f->glBegin(GL_POLYGON);
  f->glNormal3f( normal.x(), normal.y(), normal.z() );
  for( auto const &v : region ) {
    f->glVertex3f( v.x(), v.y(), v.z() );
    }
  f->glEnd();
  }



static void drawModel( QOpenGLFunctions_2_0 *f, SdM3dModel model )
  {
  for( auto const &face : model ) {
    Sd3dDraw::color( f, face.mColor );
    drawPolygon( f, face.mContour );
    }
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
  SdM3dRegion region = SdM3dFunRegionCircle::regionCircle( static_cast<float>(radius) / 1000.0, 2.0, QVector3D( center.xmm(), center.ymm(), z ) );
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
  SdM3dRegion region = SdM3dFunRegionCircle::regionCircle( static_cast<float>(radius) / 1000.0, 2.0, QVector3D( center.xmm(), center.ymm(), z ) );
  drawPolygon( f, region );
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






void Sd3dDraw::padCircle(QOpenGLFunctions_2_0 *f, SdPoint padCenter, int padRadius, QColor padColor, SdPoint holeCenter, int holeDiametr, int holeLenght, QColor holeColor, float z )
  {
  SdM3dRegion padRegion = SdM3dFunRegionCircle::regionCircle( static_cast<float>(padRadius) / 1000.0, 2.0, QVector3D( padCenter.xmm(), padCenter.ymm(), z ) );
  SdM3dRegion holeRegion;
  if( holeLenght > 0 ) {
    //Rectangle hole
    holeRegion = SdM3dFunRegionRect::regionRect( static_cast<float>(holeLenght) / 1000.0, static_cast<float>(holeDiametr) / 1000.0, QVector3D( holeCenter.xmm(), holeCenter.ymm(), z ) );
    }
  else {
    //Circle hole
    holeRegion = SdM3dFunRegionCircle::regionCircle( static_cast<float>(holeDiametr) / 2000.0, 2.0, QVector3D( padCenter.xmm(), padCenter.ymm(), z ) );
    }
  //Top pad
  SdM3dModel topPad = SdM3dFunModelHole::modelHole( padRegion, holeRegion, padColor );
  //Bottom pad
  SdM3dModel botPad = SdM3dFunModelTranslate::modelTranslate( topPad, QVector3D(0,0,z-1.6) );
  //Hole tube
  SdM3dModel hole = SdM3dFunModelExtrude::modelExtrude( holeRegion, -1.6, holeColor );
  drawModel( f, topPad );
  drawModel( f, botPad );
  drawModel( f, hole );
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

