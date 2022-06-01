/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d object reader for stl files
*/
#ifndef SD3DREADERSTL_H
#define SD3DREADERSTL_H

#include "objects/Sd3dGraphModel.h"

class Sd3dReaderStl
  {
  public:
    //!
    //! \brief importStlFromFile Read model from STL file which represented by its path
    //! \param fname             Full path to STL file
    //! \return                  Pointer to Sd3dFaceSet object if import was successfull or nullptr in other case
    //!
    static Sd3dGraphModel *importStlFromFile( QString fname );
  };

#endif // SD3DREADERSTL_H
