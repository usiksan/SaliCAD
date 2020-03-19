/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D triangle
*/
#ifndef SD3DTRIANGLE_H
#define SD3DTRIANGLE_H

#include "Sd3dPoint.h"

class QIODevice;

class Sd3dTriangle
  {
    Sd3dPoint mA, mB, mC; //! Triangle vertex [Вершины треугольника]
    Sd3dPoint mNormal;    //! Face normal of triangle [Нормаль, показывающая переднюю грань треугольника]
    quint32   mColor;     //! Face color of triangle
  public:
    Sd3dTriangle();

    //!
    //! \brief readStl Read triangle STL description from file
    //! \param file    File from which need to read triangle description
    //!
    void        readStl( QIODevice *file );

    //!
    //! \brief paint Draw triangle with OpenGL
    //! \param f     Pointer to OpenGL functions
    //!
    void        paint( QOpenGLFunctions_2_0 *f ) const;

    //!
    //! \brief write Write triangle to json file
    //! \return      JSON object with triangle
    //!
    QJsonObject write() const;

    //!
    //! \brief read Read triangle from json file
    //! \param obj  JSON object with triangle
    //!
    void        read( const QJsonObject &obj );
  private:
    //!
    //! \brief readStlVector3d Read 3d vector from STL file (3 values)
    //! \param file            STL file
    //! \return                Readed vector converted to mcm
    //!
    static Sd3dPoint readStlVector3d( QIODevice *file );

    //!
    //! \brief readStlFloat Read standard IEEE 32bit float from STL file
    //! \param file         STL file
    //! \return             Readed float
    //!
    static float     readStlFloat( QIODevice *file );

    //!
    //! \brief readStlUShort Read unsigned short from STL file
    //! \param file          STL file
    //! \return              Readed unsigned short
    //!
    static quint16   readStlUShort( QIODevice *file );

  };

#endif // SD3DTRIANGLE_H
