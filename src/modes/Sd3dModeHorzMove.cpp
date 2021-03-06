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
#include "SdModeIdents.h"
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
    mScale = viewer->scale() / 13.0;
    }
  else if( event->button() == Qt::RightButton )
    viewer->modeCancel();
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
    mOffsetX *= mScale;

    mOffsetY = -(event->pos().y() - mStartPoint.y());
    mOffsetY *= mScale;
    viewer->update();
    return true;
    }
  return false;
  }




bool Sd3dModeHorzMove::draw3d(QOpenGLFunctions_2_0 *f)
  {
  f->glPushMatrix();
  if( mLeftPressed )
    f->glTranslatef( mOffsetX, mOffsetY, 0 );

  mPart->draw3d( f );

  f->glPopMatrix();
  return true;
  }




int Sd3dModeHorzMove::modeId() const
  {
  return MD_3D_HORZ_MOVE;
  }




QString Sd3dModeHorzMove::getStepHelp() const
  {
  return QObject::tr("Press Left button and move to horizontal moving model");
  }



QString Sd3dModeHorzMove::getModeThema() const
  {
  return  QString( MODE_HELP "3dModeHorzMove.htm" );
  }
