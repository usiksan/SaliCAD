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
#include "vrml/SdScanerVrml.h"
#include "vrml/VrmlNodeMaterial.h"

#include <QMessageBox>


//!
//! \brief importVrmlFromFile Read model from VRML file which represented by its path
//! \param fname              Full path to VRML file
//! \return                   Pointer to Sd3dFaceSet object if import was successfull or nullptr in other case
//!
Sd3dFaceSet *Sd3dReaderVrml::importVrmlFromFile(QString fname, QWidget *parent)
  {
  SdScanerVrml scanerVrml;
  if( scanerVrml.parseFile(fname) ) {
    //Generate faces

    //3d object to put faces in
    Sd3dFaceSet *faceSet = new Sd3dFaceSet();

    //Generation
    scanerVrml.generateFaces( [faceSet] ( const QVector3DList &vertexList, const QVector3DList &normalList, const VrmlNodeMaterial *material ) {
      //Convert from VrmlNodeMaterial to Sd3dFaceMaterial
      Sd3dFaceMaterial faceMaterial;
      if( material != nullptr ) {
        faceMaterial.setAmbientIntensity( material->ambientIntensity() );
        faceMaterial.setDiffuseColor( material->diffuseColor(0), material->diffuseColor(1), material->diffuseColor(2) );
        faceMaterial.setEmissiveColor( material->emissiveColor(0), material->emissiveColor(1), material->emissiveColor(2) );
        faceMaterial.setShininnes( material->shininnes() );
        faceMaterial.setSpecularColor( material->specularColor(0), material->specularColor(1), material->specularColor(2) );
        faceMaterial.setTransparency( material->transparency() );
        }
      faceSet->faceAdd( Sd3dFace( vertexList, normalList, faceMaterial ) );
      });

    //Return 3d object
    return faceSet;
    }
  QMessageBox::warning( parent, QObject::tr("Error happens when read VRML file!"), scanerVrml.errorGet() );
  return nullptr;
  }