/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d object reader for VRML files
*/
#ifndef SD3DREADERVRML_H
#define SD3DREADERVRML_H

#include "Sd3dFaceSet.h"

class Sd3dReaderVrml
  {
  public:

    //!
    //! \brief importVrmlFromFile Read model from VRML file which represented by its path
    //! \param fname              Full path to VRML file
    //! \param parent             Parent widget. Is used to display messages and progress bar
    //! \return                   Pointer to Sd3dFaceSet object if import was successfull or nullptr in other case
    //!
    static Sd3dFaceSet *importVrmlFromFile( QString fname, QWidget *parent );
  };

#endif // SD3DREADERVRML_H
