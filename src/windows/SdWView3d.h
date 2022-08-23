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
#include "modes/Sd3dMode.h"

#include <QOpenGLWidget>

class SdProjectItem;


class SdWView3d : public QOpenGLWidget
  {
    Q_OBJECT

    double           mAngleZ;         //!< Angle view vector over Z axiz
    double           mAngleXY;        //!< Angle view vector over XY area
    bool             mLeftPressed;    //!< True if mouse left button pressed
    bool             mMiddlePressed;  //!< True if mouse middle button pressed
    QPoint           mStartPos;       //!< Start point of cursor when mouse button pressed
    QPoint           mPrevOrigin;     //!< Previous origin of view
    double           mPrevAngleZ;     //!< Previous angle view vector over Z axiz
    double           mPrevAngleXY;    //!< Previous angle view vector over XY area
    SdScaler         mScale;          //!< Current scale [Текущий масштаб изображения]
    SdPoint          mOrigin;         //!< Logical origin point [Логическая точка центра изображения]
    SdProjectItem   *mItem;           //!< Current viewed item
    Sd3dMode        *mMode;           //!< Currently actived interactive mode
    bool             mEnable2d;       //!< If enabled then 2d graphics draw with 3d
    bool             mEnablePad;      //!< If enabled then pad stack draws with 3d
  public:
    SdWView3d( SdProjectItem *item, QWidget *parent );
    virtual ~SdWView3d();

    //!
    //! \brief item Returns currently viewed project item
    //! \return     Currently viewed project item
    //!
    SdProjectItem   *item() { return mItem; }

    //!
    //! \brief setItem Sets new currently viewed project item
    //! \param it      New currently viewed project item
    //!
    void             setItem( SdProjectItem *it );

    //!
    //! \brief fitItem Calculate scale so that item fill all space of view
    //!
    void             fitItem();

    //!
    //! \brief modeSet Setup new active 3d mode
    //! \param mode    New active 3d mode
    //!
    void             modeSet( Sd3dMode *mode );

    //!
    //! \brief modeCancel Sets modeView as current 3d mode
    //!
    void             modeCancel();

    //!
    //! \brief mode Returns current 3d mode
    //! \return     Current 3d mode
    //!
    Sd3dMode        *mode() const { return mMode; }

    //!
    //! \brief scale Returns scale coef used to convert screen coord to phisical coord of models
    //! \return      Scale coef
    //!
    float            scale() const;
    // QWidget interface
  public:
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

  public slots:
    //!
    //! \brief setEnable2d Enables (disables) show 2d graphics at z = 0 level of 3d picture
    //! \param ena         If true then 2d graphics show
    //!
    void setEnable2d( bool ena );

    //!
    //! \brief setEnablePad Enables (disables) show pads graphics on 3d picture. Pads are shown as 3d models
    //! \param ena          If true then pads show
    //!
    void setEnablePad( bool ena );
  };

#endif // SDWVIEW3D_H
