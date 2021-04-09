/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d mode used to rotate model in horizontal area
*/
#include "Sd3dModeHorzRotate.h"
#include "SdModeIdents.h"
#include "windows/SdWView3d.h"

Sd3dModeHorzRotate::Sd3dModeHorzRotate(SdPItemPart *part) :
  Sd3dModeMat(part),
  mLeftPressed(false)
  {

  }




void Sd3dModeHorzRotate::mousePressEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  if( event->button() == Qt::LeftButton ) {
    mLeftPressed = true;
    mStartPoint = event->pos();
    mAngleX = mAngleZ = 0;
    }
  else if( event->button() == Qt::RightButton )
    viewer->modeCancel();
  }




void Sd3dModeHorzRotate::mouseReleaseEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  if( event->button() == Qt::LeftButton ) {
    mLeftPressed = false;
    //Apply rotation
    QMatrix4x4 matrix;
    matrix.rotate( mAngleZ, 0, 0, 1 );
    matrix.rotate( mAngleX, 1, 0, 0 );
    mPart->matrixMap( matrix, mPart->getUndo() );
    viewer->update();
    }
  }




bool Sd3dModeHorzRotate::mouseMoveEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  if( mLeftPressed ) {
    int deltaZ = (event->pos().x() - mStartPoint.x()) / 200;
    mAngleZ = deltaZ * 45;

    int deltaX = (event->pos().y() - mStartPoint.y()) / 200;
    mAngleX = deltaX * 45;
    viewer->update();
    return true;
    }
  return false;
  }




bool Sd3dModeHorzRotate::draw3d(QOpenGLFunctions_2_0 *f)
  {
  f->glPushMatrix();
  if( mLeftPressed ) {
    f->glRotatef( mAngleZ, 0, 0, 1 );
    f->glRotatef( mAngleX, 1, 0, 0 );
    }

  mPart->draw3d( f );

  f->glPopMatrix();
  return true;
  }




int Sd3dModeHorzRotate::modeId() const
  {
  return MD_3D_HORZ_MOVE;
  }




QString Sd3dModeHorzRotate::getStepHelp() const
  {
  return QObject::tr("Press Left button and move to rotate model around Z and X axis");
  }




QString Sd3dModeHorzRotate::getModeThema() const
  {
  return  QString( MODE_HELP "3dModeHorzRotate.htm" );
  }
