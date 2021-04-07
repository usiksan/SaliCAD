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
  Sd3dRegion region = sd3dRegionCircle( static_cast<float>(radius) / 1000.0, 2.0, QVector3D( center.xmm(), center.ymm(), z ) );
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
  Sd3dRegion region = sd3dRegionCircle( static_cast<float>(radius) / 1000.0, 2.0, QVector3D( center.xmm(), center.ymm(), z ) );
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





static void pad( QOpenGLFunctions_2_0 *f, Sd3dRegion padRegion, SdPoint holeCenter, int holeDiametr, int holeLenght, float z )
  {
  Sd3dRegion holeRegion;
  if( holeLenght > 0 )
    //Rectangle hole
    holeRegion = sd3dRegionRectangleSideCount( static_cast<float>(holeLenght) / 1000.0, static_cast<float>(holeDiametr) / 1000.0, 36, QVector3D( holeCenter.xmm(), holeCenter.ymm(), z ) );
  else
    //Circle hole
    holeRegion = sd3dRegionCircleSideCount( static_cast<float>(holeDiametr) / 2000.0, 2.0, QVector3D( holeCenter.xmm(), holeCenter.ymm(), z ) );
  //Top pad
  Sd3dModel topPad = sd3dModelFlatHole( padRegion, holeRegion, sdEnvir->getSysColor( sc3dPadTop ) );
  //Bottom pad
  Sd3dModel botPad = sd3dModelTranslateColor( topPad, QVector3D(0,0,-1.5), sdEnvir->getSysColor( sc3dPadBot ) );
  //Hole tube
  Sd3dModel hole = sd3dModelWall( holeRegion, QVector3D(0,0,-1.5), sdEnvir->getSysColor( sc3dPadHole ), true );
  Sd3dDraw::drawModel( f, topPad );
  Sd3dDraw::drawModel( f, botPad );
  Sd3dDraw::drawModel( f, hole );
  }



//!
//! \brief padCircle   Draws circle pad with hole
//! \param f           OpenGL
//! \param padCenter   Point of pad center in 3d scene
//! \param padRadius   Pad radius in micron
//! \param holeCenter  Center of pad hole aligned to center of pad
//! \param holeDiametr Hole diametr in micron
//! \param holeLenght  Lenght of hole in micron
//! \param z           z position of pad ahead pcb
//!
void Sd3dDraw::padCircle(QOpenGLFunctions_2_0 *f, SdPoint padCenter, int padRadius, SdPoint holeCenter, int holeDiametr, int holeLenght, float z )
  {
  Sd3dRegion padRegion( sd3dRegionCircleSideCount( static_cast<float>(padRadius) / 1000.0, 36, QVector3D( padCenter.xmm(), padCenter.ymm(), z ) )  );
  pad( f, padRegion, holeCenter, holeDiametr, holeLenght, z );
  }




//!
//! \brief padRect     Draws rectangle pad with hole
//! \param f           OpenGL
//! \param padCenter   Point of pad center in 3d scene
//! \param padWidth    Pad width in micron
//! \param padHeight   Pad height in micron
//! \param holeCenter  Center of pad hole aligned to center of pad
//! \param holeDiametr Hole diametr in micron
//! \param holeLenght  Lenght of hole in micron
//! \param z           z position of pad ahead pcb
//!
void Sd3dDraw::padRect(QOpenGLFunctions_2_0 *f, SdPoint padCenter, int padWidth, int padHeight, SdPoint holeCenter, int holeDiametr, int holeLenght, float z)
  {
  Sd3dRegion padRegion( sd3dRegionRectangleSideCount( static_cast<float>(padWidth) / 1000.0, static_cast<float>(padHeight) / 1000.0, 36, QVector3D( padCenter.xmm(), padCenter.ymm(), z ) ) );
  pad( f, padRegion, holeCenter, holeDiametr, holeLenght, z );
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
  Sd3dRegion region;
  region.reserve( list.count() );
  float fz(z);
  fz /= 1000.0;
  fz -= 0.22;
  for( auto const &p : list ) {
    float x(p.x());
    x /= 1000.0;
    float y(p.y());
    y /= 1000.0;
    QVector3D v( x, y, -0.11 );
    region.append( v );
    }
  Sd3dModel pcb = sd3dModelExtrude( region, -fz, color );
  drawModel( f, pcb );
  }




//!
//! \brief face Draw face in 3d world
//! \param f    OpenGL
//! \param face Face to draw
//!
void Sd3dDraw::drawFace(QOpenGLFunctions_2_0 *f, const Sd3dFace &face)
  {
  Sd3dDraw::color( f, face.mColor );
  drawPolygon( f, face.mContour );
  }




//!
//! \brief drawModel Draw model in 3d world
//! \param f         OpenGL
//! \param model     Model to draw
//!
void Sd3dDraw::drawModel(QOpenGLFunctions_2_0 *f, const Sd3dModel &model)
  {
  for( auto const &face : model )
    drawFace( f, face );
  }

