#ifndef SDWVIEW3D_H
#define SDWVIEW3D_H

#include <QOpenGLWidget>

class SdProjectItem;

class SdWView3d : public QOpenGLWidget
  {
    Q_OBJECT

    SdProjectItem *mItem;
  public:
    SdWView3d( SdProjectItem *item, QWidget *parent );

    // QOpenGLWidget interface
  protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
  };

#endif // SDWVIEW3D_H
