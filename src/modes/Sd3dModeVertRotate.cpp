/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d mode used to rotate model in vertical area
*/
#include "Sd3dModeVertRotate.h"
#include "SdModeIdents.h"
#include "windows/SdWView3d.h"

Sd3dModeVertRotate::Sd3dModeVertRotate(SdPItemPart *part) :
  Sd3dModeMat(part),
  mLeftPressed(false)
  {

  }





void Sd3dModeVertRotate::mousePressEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  if( event->button() == Qt::LeftButton ) {
    mLeftPressed = true;
    mStartPoint = event->pos();
    mAngleX = mAngleY = 0;
    }
  else if( event->button() == Qt::RightButton )
    viewer->modeCancel();
  }





void Sd3dModeVertRotate::mouseReleaseEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  if( event->button() == Qt::LeftButton ) {
    mLeftPressed = false;
    //Apply rotation
    QMatrix4x4 matrix;
    matrix.rotate( mAngleY, 0, 1, 0 );
    matrix.rotate( mAngleX, 1, 0, 0 );
    mPart->matrixMap( matrix, mPart->getUndo() );
    viewer->update();
    }
  }




bool Sd3dModeVertRotate::mouseMoveEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  if( mLeftPressed ) {
    int deltaY = (event->pos().x() - mStartPoint.x()) / 200;
    mAngleY = deltaY * 45;

    int deltaX = (event->pos().y() - mStartPoint.y()) / 200;
    mAngleX = deltaX * 45;
    viewer->update();
    return true;
    }
  return false;
  }




bool Sd3dModeVertRotate::draw3d(QOpenGLFunctions_2_0 *f)
  {
  f->glPushMatrix();
  if( mLeftPressed ) {
    f->glRotatef( mAngleY, 0, 1, 0 );
    f->glRotatef( mAngleX, 1, 0, 0 );
    }

  mPart->draw3d( f );

  f->glPopMatrix();
  return true;
  }




int Sd3dModeVertRotate::modeId() const
  {
  return MD_3D_VERT_ROTATE;
  }




QString Sd3dModeVertRotate::getStepHelp() const
  {
  return QObject::tr("Press Left button and move to rotate model around Y and X axis");
  }




QString Sd3dModeVertRotate::getModeThema() const
  {
  return  QString( MODE_HELP "3dModeVertRotate.htm" );
  }
