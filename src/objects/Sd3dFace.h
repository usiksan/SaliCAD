/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D face. Face is flat surface bounded by region
*/
#ifndef SD3DFACE_H
#define SD3DFACE_H

#include "Sd3dRegion.h"

#include <QColor>
#include <QJsonObject>

struct Sd3dFace
  {
    Sd3dRegion mContour; //!< Face contour
    QColor     mColor;   //!< Face color

    //!
    //! \brief write Writes face to JSON object
    //! \return      JSON object with face
    //!
    QJsonObject write() const;

    //!
    //! \brief read Read face from JSON object
    //! \param obj  JSON object with face
    //!
    void        read( const QJsonObject &obj );
  };

#endif // SD3DFACE_H
