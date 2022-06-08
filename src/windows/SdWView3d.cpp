/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Real 3d view widget.
*/
#include "SdWView3d.h"
#include "SdWCommand.h"
#include "objects/Sd3dGraphModel.h"
#include "objects/SdProjectItem.h"
#include "objects/SdPulsar.h"
#include "modes/Sd3dModeView.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_2_0>
#include <QWheelEvent>
#include <QDebug>

SdWView3d::SdWView3d(SdProjectItem *item, QWidget *parent) :
  QOpenGLWidget( parent ),
  mAngleZ(13.0),
  mAngleXY(-51.0),
  mLeftPressed(false),    //!< True if mouse left button pressed
  mMiddlePressed(false),  //!< True if mouse middle button pressed
  mScale(0.1),
  mItem(item),
  mMode( nullptr ),
  mEnable2d(true),
  mEnablePad(true)
  {
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(2, 0);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setSamples(2);

  setFormat( format );

  modeSet( new Sd3dModeView() );
  }




SdWView3d::~SdWView3d()
  {
  if( mMode != nullptr )
    delete mMode;
  }



void SdWView3d::setItem(SdProjectItem *it)
  {
  mItem = it;
  update();
  }




void SdWView3d::fitItem()
  {
  //Calculate view volume
  if( mItem != nullptr ) {
    //Volume for volume calculation
    QMatrix2x3 volume;
    //Init volume
    volume(0,0) = volume(1,0) = volume(2,0) =  1000000.0;
    volume(0,1) = volume(1,1) = volume(2,1) = -1000000.0;
    //Append volume for each 3d object
    mItem->forEachConst( dct3D, [&volume] (SdObject *obj) -> bool {
      SdPtrConst<Sd3dGraph> obj3d(obj);
      if( obj3d.isValid() )
        obj3d->volumeAdd( volume );
      return true;
      });

    //Calculate scale
    float scalex = (volume(0,1) - volume(0,0));
    float scaley = (volume(1,1) - volume(1,0));
    float scalez = (volume(2,1) - volume(2,0));
    if( scalex > 0 && scaley > 0 && scalez > 0 ) {
      scalex = 45.0 / scalex;
      scaley = 45.0 / scaley;
      scalez = 45.0 / scalez;
      mScale.scaleSet( qMin( scalex, qMin(scaley,scalez)) );
      update();
      }
    }
  }




//!
//! \brief modeSet Setup new active 3d mode
//! \param mode    New active 3d mode
//!
void SdWView3d::modeSet(Sd3dMode *mode)
  {
  //qDebug() << "mode set" << mode->modeId();
  //Remove previous mode
  if( mMode != nullptr )
    delete mMode;
  mMode = mode;
  //Activate mode's command icon
  SdWCommand::selectMode( mMode->modeId() );
  //Set default status message for activated mode
  SdPulsar::sdPulsar->emitSetStatusMessage( mMode->getStepHelp() );
  }




//!
//! \brief modeCancel Sets modeView as current 3d mode
//!
void SdWView3d::modeCancel()
  {
  modeSet( new Sd3dModeView() );
  }




//!
//! \brief scale Returns scale coef used to convert screen coord to phisical coord of models
//! \return      Scale coef
//!
float SdWView3d::scale() const
  {
  return 1.0 / mScale.scaleGet();
  }






void SdWView3d::mousePressEvent(QMouseEvent *event)
  {
  //At first try handle event by current mode
  if( mMode != nullptr )
    mMode->mousePressEvent( this, event );

  //If no current mode or mode does not handle event then we handle it self
  if( event->button() == Qt::LeftButton ) {
    mLeftPressed = true;
    mStartPos = event->pos();
    mPrevAngleXY = mAngleXY;
    mPrevAngleZ = mAngleZ;
    }
  else if( event->button() == Qt::MiddleButton ) {
    mMiddlePressed = true;
    mStartPos = event->pos();
    mPrevOrigin = mOrigin;
    }
  }



void SdWView3d::mouseReleaseEvent(QMouseEvent *event)
  {
  //At first try handle event by current mode
  if( mMode != nullptr )
    mMode->mouseReleaseEvent( this, event );

  //If no current mode or mode does not handle event then we handle it self
  if( event->button() == Qt::LeftButton )
    mLeftPressed = false;
  else if( event->button() == Qt::MiddleButton )
    mMiddlePressed = false;
  }



void SdWView3d::mouseMoveEvent(QMouseEvent *event)
  {
  //At first try handle event by current mode
  if( mMode != nullptr && mMode->mouseMoveEvent( this, event ) )
    return;

  //If no current mode or mode does not handle event then we handle it self
  if( mLeftPressed ) {
    mAngleXY = mPrevAngleXY - (mStartPos.y() - event->pos().y()) / 4;
    mAngleZ = mPrevAngleZ - (mStartPos.x() - event->pos().x()) / 4;
    update();
    }
  else if( mMiddlePressed ) {
    mOrigin = mPrevOrigin + QPoint( event->pos().x() - mStartPos.x(), mStartPos.y() - event->pos().y() ) / 2;
    update();
    }
  }



int wheelDelta = 50;

void SdWView3d::wheelEvent(QWheelEvent *event)
  {
#if 1
  //At first try handle event by current mode
  if( mMode != nullptr && mMode->wheelEvent( this, event ) )
    return;

  int delta = event->angleDelta().y() / 120;
  //qDebug() << "wheel" << event->angleDelta().y();
  if( event->modifiers() & Qt::ControlModifier ) {
    //With Control pressed we rotate over area XY
    mAngleXY += delta;
    }
  else if( event->modifiers() & Qt::ShiftModifier ) {
    //With Shift pressed we change Scale
    if( event->angleDelta().y() < 0 ) mScale.scaleStep(1.2);
    else mScale.scaleStep(0.83);
    //qDebug() << "scale" << mScale.scaleGet();
    }
  else {
    //With no modifiers pressed we rotate over Z axis
    mAngleZ += delta;
    }
#else
  wheelDelta += event->angleDelta().y() / 120;
  initializeGL();
#endif
  update();
  }







void SdWView3d::keyPressEvent(QKeyEvent *event)
  {
  //Help key
  if( event->key() == Qt::Key_F1 ) {
    if( mMode != nullptr )
      SdPulsar::sdPulsar->emitHelpTopic( mMode->getModeThema() );
    }
  //At first try handle event by current mode
  if( mMode != nullptr )
    mMode->keyPressEvent( this, event );
  }





void SdWView3d::keyReleaseEvent(QKeyEvent *event)
  {
  //At first try handle event by current mode
  if( mMode != nullptr )
    mMode->keyReleaseEvent( this, event );
  }





void SdWView3d::initializeGL()
  {
//  float light = qBound( 0, wheelDelta, 50 );
//  qDebug() << "Wheel delta" << wheelDelta;

  // Set up the rendering context, load shaders and other resources, etc.:
  QOpenGLFunctions_2_0 *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_2_0>();
  f->glClearColor(0.8f, 0.93f, 0.93f, 1.0f);
  f->glShadeModel( GL_SMOOTH );

  f->glEnable( GL_LIGHTING );
  f->glEnable( GL_LIGHT0 );
  f->glEnable( GL_DEPTH_TEST );

  f->glEnable(GL_MULTISAMPLE);
  // автоматическое приведение нормалей к
  // единичной длине
  f->glEnable(GL_NORMALIZE);

  //GLfloat light_position[] = { 0.0, -1000.0, 1000.0, 0.0 };
  GLfloat light_diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
  GLfloat light_ambient[] = { 0.9f, 0.9f, 0.9f, 1.0f };
  GLfloat model_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };

  //First light
  //f->glLightfv( GL_LIGHT0, GL_POSITION, light_position );
  f->glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
  f->glMaterialfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
  f->glLightModelfv( GL_LIGHT_MODEL_AMBIENT, model_ambient );


  GLfloat light1_diffuse[] = {0.6, 0.6, 0.6, 1.0 };
  //GLfloat light1_position[] = {0.0, -1000.0, -1000.0, 1.0};
  //GLfloat light2_position[] = {-1000.0, 1000.0, 1000.0, 1.0};
  //GLfloat light3_position[] = { 1000.0, 1000.0, 1000.0, 1.0};
  f->glEnable(GL_LIGHT1);
  f->glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  //f->glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

  f->glEnable(GL_LIGHT2);
  f->glLightfv(GL_LIGHT2, GL_DIFFUSE, light1_diffuse);
  //f->glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

  f->glEnable(GL_LIGHT3);
  f->glLightfv(GL_LIGHT3, GL_DIFFUSE, light1_diffuse);
  //f->glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
  }





void SdWView3d::resizeGL(int w, int h)
  {
  //qDebug() << "resize 3d";
  QOpenGLFunctions_2_0 *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_2_0>();
  f->glViewport( 0, 0, w, h );
  double k = ((double)w) / ((double)h);
  f->glMatrixMode( GL_PROJECTION );
  f->glLoadIdentity();
  f->glOrtho(-50.0 * k, 50.0 * k, -50.0, 50.0, -1000.0, 1000.0);

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


  //First light
  GLfloat light_position[] = { -1000.0, 0.0, 1000.0, 1.0 };
  f->glLightfv( GL_LIGHT0, GL_POSITION, light_position );

  GLfloat light1_position[] = { 1000.0, 0.0,  1000.0, 1.0};
  f->glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

  GLfloat light2_position[] = { 0.0, -1000.0, -1000.0, 1.0};
  f->glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

  GLfloat light3_position[] = { 0.0, 1000.0, -1000.0, 1.0};
  f->glLightfv(GL_LIGHT3, GL_POSITION, light3_position);

  f->glClear(GL_COLOR_BUFFER_BIT);



  //Material used for draw 2d graphics and axis
  Sd3dMaterial axisMaterial;

  //Axis
  axisMaterial.mDiffuse = QColor( 255, 255, 0 );
  axisMaterial.draw( f );
  //f->glLineWidth(3);
  f->glBegin( GL_LINES );
  f->glVertex3d( -1000.0, 0, 0 );
  f->glVertex3d( 1000.0, 0, 0 );

  f->glVertex3d( 0, -1000.0, 0 );
  f->glVertex3d( 0, 1000.0, 0 );
  f->glEnd();


  if( mItem != nullptr ) {
    //2d graphics
    if( mEnable2d ) {
      axisMaterial.mDiffuse = QColor( 255, 0, 0 );
      axisMaterial.draw( f );
      quint64 mask = dctLines;
      if( mEnablePad ) mask |= dctPartPin;
      mItem->forEachConst( mask, [f] (SdObject *obj) -> bool {
        SdPtrConst<SdGraph> obj2d(obj);
        if( obj2d.isValid() )
          obj2d->draw3d( f );
        return true;
        });
      }

    //Draw throught mode if mode present
    if( mMode == nullptr || !mMode->draw3d( f ) )
      //or use direct item draw if no mode present or mode not drawn
      mItem->draw3d( f );
    }

  //f->glFlush();
  }




void SdWView3d::setEnable2d(bool ena)
  {
  mEnable2d = ena;
  update();
  }




void SdWView3d::setEnablePad(bool ena)
  {
  mEnablePad = ena;
  update();
  }
