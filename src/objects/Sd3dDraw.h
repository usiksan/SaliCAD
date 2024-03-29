/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Utilites for common 3d drawing
*/
#ifndef SD3DDRAW_H
#define SD3DDRAW_H

#include "SdPointList.h"
#include "SdRect.h"

#include <QOpenGLFunctions_2_0>

class Sd3dDraw
  {
  public:
    //!
    //! \brief line Draw line segment
    //! \param f    OpenGL
    //! \param a    Start point of segment
    //! \param b    Finish point of segment
    //! \param z    z level in mm
    //!
    static void line( QOpenGLFunctions_2_0 *f, SdPoint a, SdPoint b, float z );

    //!
    //! \brief rect Draw rectangle region with 4 lines
    //! \param f    OpenGL
    //! \param a    First corner point
    //! \param b    Opposite corner point
    //! \param z    z level in mm
    //!
    static void rect( QOpenGLFunctions_2_0 *f, SdPoint a, SdPoint b, float z );

    //!
    //! \brief rect Draw filled rectangle polygon
    //! \param f    OpenGL
    //! \param a    First corner point
    //! \param b    Opposite corner point
    //! \param z    z level in mm
    //!
    static void rectFilled( QOpenGLFunctions_2_0 *f, SdPoint a, SdPoint b, float z );


    //!
    //! \brief circle Draw circle as mulcorner region
    //! \param f      OpenGL
    //! \param center Center of circle
    //! \param radius Radius of circle in mcm
    //! \param z      z level in mm
    //!
    static void circle( QOpenGLFunctions_2_0 *f, SdPoint center, int radius, float z );

    //!
    //! \brief circle Draw circle as filled multicorner polygon
    //! \param f      OpenGL
    //! \param center Center of circle
    //! \param radius Radius of circle in mcm
    //! \param z      z level in mm
    //!
    static void circleFill( QOpenGLFunctions_2_0 *f, SdPoint center, int radius, float z );

    //!
    //! \brief color Setup current color
    //! \param f     OpenGL
    //! \param col   New current color
    //!
    static void color( QOpenGLFunctions_2_0 *f, QColor col );

    //!
    //! \brief colorToFloat Convert QColor to array of four float color components
    //! \param col          Source color
    //! \param fcolor       Destignation array of float, must be at least 4 item
    //!
    static void colorToFloat(QColor col, float *fcolor );

  };

#endif // SD3DDRAW_H
