/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base class for all 3d view modes
*/
#ifndef SD3DMODE_H
#define SD3DMODE_H

#include <QMouseEvent>
#include <QOpenGLFunctions_2_0>


class SdWView3d;

class Sd3dMode
  {
  public:
    Sd3dMode();
    virtual ~Sd3dMode() {}

    virtual void mousePressEvent( SdWView3d *viewer, QMouseEvent *event );
    virtual void mouseReleaseEvent( SdWView3d *viewer, QMouseEvent *event );
    virtual bool mouseMoveEvent( SdWView3d *viewer, QMouseEvent *event );
    virtual bool wheelEvent( SdWView3d *viewer, QWheelEvent *event );
    virtual void keyPressEvent( SdWView3d *viewer, QKeyEvent *event );
    virtual void keyReleaseEvent( SdWView3d *viewer, QKeyEvent *event );
    virtual bool draw3d(QOpenGLFunctions_2_0 *f) = 0;
    virtual int  modeId() const = 0;

    //!
    //! \brief getStepHelp Return short help on current mode step
    //! \return            Short help on current mode step
    //!
    virtual QString getStepHelp() const = 0;

    //!
    //! \brief getModeThema Returns Help page address [Адрес страницы помощи]
    //! \return                     Help page address
    //!
    virtual QString getModeThema() const = 0;
  };

#endif // SD3DMODE_H
