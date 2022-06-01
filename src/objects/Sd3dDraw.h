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
#include "Sd3dModel.h"

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

    //!
    //! \brief padCircle   Draws circle pad with hole
    //! \param f           OpenGL
    //! \param padCenter   Point of pad center in 3d scene
    //! \param padRadius   Pad radius in micron
    //! \param holeCenter  Center of pad hole aligned to center of pad
    //! \param holeDiametr Hole diametr in micron
    //! \param holeLenght  Lenght of hole in micron
    //! \param z           z position of pad ahead pcb
    //!
    static void padCircle(QOpenGLFunctions_2_0 *f, SdPoint padCenter, int padRadius, SdPoint holeCenter, int holeDiametr, int holeLenght, float z);

    //!
    //! \brief padRect     Draws rectangle pad with hole
    //! \param f           OpenGL
    //! \param padCenter   Point of pad center in 3d scene
    //! \param padWidth    Pad width in micron
    //! \param padHeight   Pad height in micron
    //! \param holeCenter  Center of pad hole aligned to center of pad
    //! \param holeDiametr Hole diametr in micron
    //! \param holeLenght  Lenght of hole in micron
    //! \param z           z position of pad ahead pcb
    //!
    static void padRect(QOpenGLFunctions_2_0 *f, SdPoint padCenter, int padWidth, int padHeight, SdPoint holeCenter, int holeDiametr, int holeLenght, float z );

    //!
    //! \brief flatPanel Draws flat panel (often pcb) by contour point list and faces color
    //! \param f         OpenGL
    //! \param list      Contour point list
    //! \param z         Thickness of flat panel in micron (negativ extruded in down)
    //! \param color     Color of flat panel
    //!
    static void flatPanel(QOpenGLFunctions_2_0 *f, SdPointList list, int z, QColor color );

    static void qube( QOpenGLFunctions_2_0 *f, SdRect rect, int z, unsigned color );

    //!
    //! \brief drawFace Draw face in 3d world
    //! \param f        OpenGL
    //! \param face     Face to draw
    //!
    static void drawFace( QOpenGLFunctions_2_0 *f, const Sd3dFace &face );

    //!
    //! \brief drawModel Draw model in 3d world
    //! \param f         OpenGL
    //! \param model     Model to draw
    //!
    static void drawModel( QOpenGLFunctions_2_0 *f, const Sd3dModel &model );
  };

#endif // SD3DDRAW_H
