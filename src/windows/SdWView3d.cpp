#include "SdWView3d.h"
#include "objects/SdContext3d.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_2_0>
#include <QDebug>

SdWView3d::SdWView3d(SdProjectItem *item, QWidget *parent) :
  QOpenGLWidget( parent ),
  mAngleZ(0.0),
  mAngleXY(30.0)
  {

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(2, 0);
  format.setProfile(QSurfaceFormat::CoreProfile);
  }

void SdWView3d::mousePressEvent(QMouseEvent *event)
  {

  }

void SdWView3d::mouseReleaseEvent(QMouseEvent *event)
  {

  }

void SdWView3d::mouseMoveEvent(QMouseEvent *event)
  {

  }

void SdWView3d::wheelEvent(QWheelEvent *event)
  {
  qDebug() << "wheel view";
  //At first try handle event with mode
  //if mode not handle event, then make defaul behavior
//  if( modeGet() && modeGet()->wheel( event->angleDelta() ) )
//    return;
  int delta = event->angleDelta().y() / 120;
  //qDebug() << "wheel" << event->angleDelta().y();
  if( event->modifiers() & Qt::ControlModifier ) {
    //Move by horizontal
    mAngleXY += delta;
    //mOrigin.move( SdPoint(delta * 20, 0) );
    //originSet( SdPoint(originGet().x() + delta * gridGet().x(), originGet().y()) );
    }
  else if( event->modifiers() & Qt::ShiftModifier ) {
    //Scale
    if( event->angleDelta().y() < 0 ) mScale.scaleStep(1.2);
    else mScale.scaleStep(0.83);
    }
  else {
    //Move by vertical
    mAngleZ += delta;
    //mOrigin.move( SdPoint(0, delta * 20) );
    //originSet( SdPoint(originGet().x(), originGet().y() + delta * gridGet().y()) );
    }
  update();
  }

void SdWView3d::keyPressEvent(QKeyEvent *event)
  {

  }

void SdWView3d::keyReleaseEvent(QKeyEvent *event)
  {
  }


void SdWView3d::initializeGL()
  {
  GLfloat light_position[] = { 0.0, 0.0, 1000.0, 0.0 };
  GLfloat light_diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
  GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat mat_shininess[] = { 50.0 };
  GLfloat model_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
  // Set up the rendering context, load shaders and other resources, etc.:
  QOpenGLFunctions_2_0 *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_2_0>();
  f->glClearColor(1.f, 1.0f, 1.0f, 1.0f);
  f->glShadeModel( GL_SMOOTH );
  f->glLightfv( GL_LIGHT0, GL_POSITION, light_position );
  f->glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
  //f->glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
  //f->glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );
  f->glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
  f->glLightModelfv( GL_LIGHT_MODEL_AMBIENT, model_ambient );

  f->glEnable( GL_LIGHTING );
  f->glEnable( GL_LIGHT0 );
  f->glEnable( GL_DEPTH_TEST );
  f->glEnable( GL_COLOR_MATERIAL );
  }

void SdWView3d::resizeGL(int w, int h)
  {
  //qDebug() << "resize 3d";
  QOpenGLFunctions_2_0 *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_2_0>();
  f->glViewport( 0, 0, w, h );
  f->glMatrixMode( GL_PROJECTION );
  f->glLoadIdentity();
  f->glOrtho(-100.0, 100.0, -100.0, 100.0, -1000.0, 1000.0);

  f->glMatrixMode( GL_MODELVIEW );
  // Update projection matrix and other size related settings:
//      m_projection.setToIdentity();
//      m_projection.perspective(45.0f, w / float(h), 0.01f, 100.0f);

  }


//static void glLine( QOpenGLFunctions_2_0 *f, double x1, double y1, double z1, double x2, double y2, double z2, double  )

void SdWView3d::paintGL()
  {
  qDebug() << "Paint 3d" << mScale.scaleGet();
  // Draw the scene:
//  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
//  f->glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
//  f->glClear(GL_COLOR_BUFFER_BIT);

  QOpenGLFunctions_2_0 *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_2_0>();
  f->glLoadIdentity();
  f->glTranslated( mOrigin.x(), mOrigin.y(), 0 );
  f->glScaled( mScale.scaleGet(), mScale.scaleGet(), mScale.scaleGet() );
  f->glRotated( mAngleXY, 1, 0, 0.0 );
  //f->glRotated( mAngleXY, 0, 1, 0.0 );
  f->glRotated( mAngleZ, 0, 0, 1 );
  f->glClear(GL_COLOR_BUFFER_BIT);
  f->glColor3d(0.6, 0.0, 0.0);
  f->glBegin(GL_POLYGON);
  f->glNormal3f( 0, 0, 1 );
  f->glVertex3f ( -25, -25, 0.0);
  f->glVertex3f (  25, -25, 0.0);
  f->glVertex3f (  25,  25, 0.0);
  f->glVertex3f ( -25,  25, 0.0);
  f->glEnd();

  f->glBegin(GL_POLYGON);
  f->glNormal3f( 0, -1, 0 );
  f->glVertex3f ( -25, -25, 0.0);
  f->glVertex3f (  25, -25, 0.0);
  f->glVertex3f (  25, -25, -10);
  f->glVertex3f ( -25, -25, -10);
  f->glEnd();


  f->glColor3d(0.3, 0.8, 0.0);
  f->glBegin(GL_POLYGON);
  f->glNormal3f( 0, 0, -1 );
  f->glVertex3f ( -25, -25, -10);
  f->glVertex3f (  25, -25, -10);
  f->glVertex3f (  25,  25, -10);
  f->glVertex3f ( -25,  25, -10);
  f->glEnd();

  //f->glLineWidth(3);
  f->glColor3d(0.0, 0.0, 1.0);
  f->glBegin( GL_LINES );
  f->glVertex3d( -1000.0, 0, 0 );
  f->glVertex3d( 1000.0, 0, 0 );
//  f->glEnd();

//  f->glBegin( GL_LINES );
  f->glVertex3d( 0, -1000.0, 0 );
  f->glVertex3d( 0, 1000.0, 0 );
  f->glEnd();
  //f->glFlush();
  }
