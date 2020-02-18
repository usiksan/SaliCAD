#include "Sd3dDraw.h"
#include <QColor>


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

