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
#include "Sd3dReaderVrml.h"
#include "SdScanerVrml.h"
#include "objects/Sd3dGraphModel.h"
#include "VrmlNodeMaterial.h"

#include <QMessageBox>




//!
//! \brief importVrml Read model from VRML file which represented by its path
//! \param fname      Full path to VRML file
//! \param parent     Parent widget. Is used to display messages and progress bar
//! \return           Pointer to Sd3dGraph object if import was successfull or nullptr in other case
//!
Sd3dGraph *Sd3dReaderVrml::importVrmlFromFile(QString fname, QWidget *parent)
  {
  SdScanerVrml scanerVrml;
  if( scanerVrml.parseFile(fname) ) {
    //Generate faces

    //3d object to put faces in
    Sd3dModel model;

    //Generation
    scanerVrml.generateFaces( &model );

    //Return 3d object
    return new Sd3dGraphModel( model );
    }
  QMessageBox::warning( parent, QObject::tr("Error happens when read VRML file!"), scanerVrml.errorGet() );
  return nullptr;
  }




