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
#ifndef SDWVIEW3D_H
#define SDWVIEW3D_H

#include "objects/SdPoint.h"
#include "objects/SdScaler.h"

#include <QOpenGLWidget>

class SdProjectItem;


class SdWView3d : public QOpenGLWidget
  {
    Q_OBJECT

    double           mAngleZ;         //!< Angle view vector over Z axiz
    double           mAngleXY;        //!< Angle view vector over XY area
    SdScaler         mScale;          //!< Current scale [Текущий масштаб изображения]
    SdPoint          mOrigin;         //!< Logical origin point [Логическая точка центра изображения]
    SdProjectItem   *mItem;           //!< Current viewed item
  public:
    SdWView3d( SdProjectItem *item, QWidget *parent );

    SdProjectItem   *item() { return mItem; }
    // QWidget interface
  protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    // QOpenGLWidget interface
  protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
  };

#endif // SDWVIEW3D_H
