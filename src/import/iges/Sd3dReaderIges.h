/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d object reader for IGES files
*/
#ifndef SD3DREADERIGES_H
#define SD3DREADERIGES_H

#include "objects/Sd3dGraphModel.h"


class Sd3dReaderIges
  {
  public:

    //!
    //! \brief importVrml Read model from VRML file which represented by its path
    //! \param fname      Full path to IGES file
    //! \param parent     Parent widget. Is used to display messages and progress bar
    //! \return           Pointer to Sd3dGraph object if import was successfull or nullptr in other case
    //!
    static Sd3dGraph   *importIgesFromFile( QString fname, QWidget *parent );
  };

#endif // SD3DREADERIGES_H
