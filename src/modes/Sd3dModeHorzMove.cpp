/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d mode used to translate model in horizontal area
*/
#include "Sd3dModeHorzMove.h"
#include "windows/SdWView3d.h"

Sd3dModeHorzMove::Sd3dModeHorzMove(SdPItemPart *part) :
  Sd3dModeMat(part),
  mLeftPressed(false)
  {

  }





void Sd3dModeHorzMove::mousePressEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  if( event->button() == Qt::LeftButton ) {
    mLeftPressed = true;
    mStartPoint = event->pos();
    mOffsetX = mOffsetY = 0;
    mScale = viewer->scale();
    }
  }




void Sd3dModeHorzMove::mouseReleaseEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  if( event->button() == Qt::LeftButton ) {
    mLeftPressed = false;
    //Apply movement
    QMatrix4x4 matrix;
    matrix.translate( mOffsetX, mOffsetY );
    mPart->matrixMap( matrix, mPart->getUndo() );
    viewer->update();
    }
  }





bool Sd3dModeHorzMove::mouseMoveEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  if( mLeftPressed ) {
    mOffsetX = (event->pos().x() - mStartPoint.x());
    mOffsetX *= mScale / 4.0;

    mOffsetY = -(event->pos().y() - mStartPoint.y());
    mOffsetY *= mScale / 4.0;
    viewer->update();
    return true;
    }
  return false;
  }




void Sd3dModeHorzMove::draw3d(QOpenGLFunctions_2_0 *f)
  {
  f->glPushMatrix();
  if( mLeftPressed )
    f->glTranslatef( mOffsetX, mOffsetY, 0 );

  mPart->draw3d( f );

  f->glPopMatrix();
  }
