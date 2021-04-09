/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d mode used to translate model in vertical area
*/
#include "Sd3dModeVertMove.h"
#include "SdModeIdents.h"
#include "windows/SdWView3d.h"

Sd3dModeVertMove::Sd3dModeVertMove(SdPItemPart *part) :
  Sd3dModeMat(part),
  mLeftPressed(false)
  {

  }





void Sd3dModeVertMove::mousePressEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  if( event->button() == Qt::LeftButton ) {
    mLeftPressed = true;
    mStartPoint = event->pos();
    mOffsetX = mOffsetZ = 0;
    mScale = viewer->scale() / 13.0;
    }
  else if( event->button() == Qt::RightButton )
    viewer->modeCancel();
  }





void Sd3dModeVertMove::mouseReleaseEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  if( event->button() == Qt::LeftButton ) {
    mLeftPressed = false;
    //Apply movement
    QMatrix4x4 matrix;
    matrix.translate( mOffsetX, 0, mOffsetZ );
    mPart->matrixMap( matrix, mPart->getUndo() );
    viewer->update();
    }
  }




bool Sd3dModeVertMove::mouseMoveEvent(SdWView3d *viewer, QMouseEvent *event)
  {
  if( mLeftPressed ) {
    mOffsetX = (event->pos().x() - mStartPoint.x());
    mOffsetX *= mScale * 0;

    mOffsetZ = -(event->pos().y() - mStartPoint.y());
    mOffsetZ *= mScale;
    viewer->update();
    return true;
    }
  return false;
  }




bool Sd3dModeVertMove::draw3d(QOpenGLFunctions_2_0 *f)
  {
  f->glPushMatrix();
  if( mLeftPressed )
    f->glTranslatef( mOffsetX, 0, mOffsetZ );

  mPart->draw3d( f );

  f->glPopMatrix();
  return true;
  }





int Sd3dModeVertMove::modeId() const
  {
  return MD_3D_VERT_MOVE;
  }





QString Sd3dModeVertMove::getStepHelp() const
  {
  return QObject::tr("Press Left button and move to vertical moving model");
  }




QString Sd3dModeVertMove::getModeThema() const
  {
  return  QString( MODE_HELP "3dModeVertMove.htm" );
  }
