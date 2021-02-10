#include "SdWView3d.h"
#include "objects/SdProjectItem.h"
#include "objects/Sd3dFaceMaterial.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_2_0>
#include <QWheelEvent>
#include <QDebug>

SdWView3d::SdWView3d(SdProjectItem *item, QWidget *parent) :
  QOpenGLWidget( parent ),
  mAngleZ(13.0),
  mAngleXY(-51.0),
  mScale(0.01),
  mItem(item)
  {

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(2, 0);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setSamples(2);

  setFormat( format );
  }

bool isPressed;
QPoint startPos;
double prevAngleXY;
double prevAngleZ;



void SdWView3d::mousePressEvent(QMouseEvent *event)
  {
  if( event->button() == Qt::LeftButton ) {
    isPressed = true;
    startPos = event->pos();
    prevAngleXY = mAngleXY;
    prevAngleZ = mAngleZ;
    }
  }



void SdWView3d::mouseReleaseEvent(QMouseEvent *event)
  {
  if( event->button() == Qt::LeftButton )
    isPressed = false;
  }



void SdWView3d::mouseMoveEvent(QMouseEvent *event)
  {
  if( isPressed ) {
    mAngleXY = prevAngleXY - (startPos.y() - event->pos().y()) / 4;
    mAngleZ = prevAngleZ - (startPos.x() - event->pos().x()) / 4;
    update();
    }
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
    qDebug() << "angle xy" << mAngleXY;
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
    qDebug() << "angle z" << mAngleZ;
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
  GLfloat light_position[] = { 0.0, 1.0, 0.0, 0.0 };
  GLfloat light_diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
  GLfloat light_ambient[] = { 0.9f, 0.9f, 0.9f, 1.0f };
  GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat mat_shininess[] = { 50.0 };
  GLfloat model_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
  // Set up the rendering context, load shaders and other resources, etc.:
  QOpenGLFunctions_2_0 *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_2_0>();
  f->glClearColor(0.8f, 0.93f, 0.93f, 1.0f);
  f->glShadeModel( GL_SMOOTH );
  f->glLightfv( GL_LIGHT0, GL_POSITION, light_position );
  f->glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
  f->glMaterialfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
  //f->glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );
  //f->glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
  f->glLightModelfv( GL_LIGHT_MODEL_AMBIENT, model_ambient );

  f->glEnable( GL_LIGHTING );
  f->glEnable( GL_LIGHT0 );
  f->glEnable( GL_DEPTH_TEST );
  //f->glEnable( GL_COLOR_MATERIAL );

  f->glEnable(GL_MULTISAMPLE);

  // автоматическое приведение нормалей к
  // единичной длине
  glEnable(GL_NORMALIZE);

  GLfloat light1_diffuse[] = {0.7, 0.7, 0.7};
  GLfloat light1_position[] = {0.0, 0.0, 1000.0, 1.0};
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  }

void SdWView3d::resizeGL(int w, int h)
  {
  //qDebug() << "resize 3d";
  QOpenGLFunctions_2_0 *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_2_0>();
  f->glViewport( 0, 0, w, h );
  f->glMatrixMode( GL_PROJECTION );
  f->glLoadIdentity();
  f->glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);

  f->glMatrixMode( GL_MODELVIEW );
  }




void SdWView3d::paintGL()
  {
  // Draw the scene:

  QOpenGLFunctions_2_0 *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_2_0>();

  f->glLoadIdentity();
  f->glTranslated( mOrigin.x(), mOrigin.y(), 0 );
  f->glScaled( mScale.scaleGet(), mScale.scaleGet(), mScale.scaleGet() );
  f->glRotated( mAngleXY, 1, 0, 0.0 );
  //f->glRotated( mAngleXY, 0, 1, 0.0 );
  f->glRotated( mAngleZ, 0, 0, 1 );
  f->glClear(GL_COLOR_BUFFER_BIT);

  mItem->draw3d( f );

  //Axis
  Sd3dFaceMaterial axisMaterial;
  axisMaterial.setDiffuseColor( 1.0, 0.0, 0.0 );
  axisMaterial.paint( f );
  //f->glLineWidth(3);
  f->glBegin( GL_LINES );
  f->glVertex3d( -1000.0, 0, 0 );
  f->glVertex3d( 1000.0, 0, 0 );

  f->glVertex3d( 0, -1000.0, 0 );
  f->glVertex3d( 0, 1000.0, 0 );
  f->glEnd();
  //f->glFlush();
  }
